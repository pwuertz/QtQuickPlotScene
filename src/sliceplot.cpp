#include "sliceplot.h"

#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>


class SlicePlotMaterial : public QSGMaterial
{
public:
    QSGMaterialType *type() const { static QSGMaterialType type; return &type; }
    QSGMaterialShader *createShader() const;
    QSGTexture* m_texture_data;
    double m_width;
    double m_height;
    double m_amplitude;
    double m_offset;
    QPointF m_p1;
    QPointF m_p2;
    QColor m_color;
};

class SlicePlotShader : public QSGMaterialShader
{
public:
    const char *vertexShader() const {
        return
        "uniform sampler2D data;                                   \n"
        "attribute highp vec2 vertex;                              \n"
        "uniform highp float width;                                \n"
        "uniform highp float height;                               \n"
        "uniform highp float amplitude;                            \n"
        "uniform highp float offset;                               \n"
        "uniform highp vec2 p1;                                    \n"
        "uniform highp vec2 p2;                                    \n"
        "uniform highp mat4 matrix;                                \n"
        "                                                          \n"
        "void main() {                                             \n"
        "    highp vec2 pos = (1.-vertex.x)*p1 + vertex.x*p2;      \n"
        "    highp float val = texture2D(data, pos).r;             \n"
        "    highp float yval = amplitude * (val+offset);          \n"
        "    gl_Position = matrix * vec4(width * vertex.x, height * yval, 0., 1.);  \n"
        "}";
    }

    const char *fragmentShader() const {
        return
        "uniform sampler2D data;                       \n"
        "uniform lowp float opacity;                   \n"
        "uniform mediump vec4 color;                   \n"
        "void main() {                                 \n"
        "    gl_FragColor = color * opacity;           \n"
        "}";
    }

    char const *const *attributeNames() const
    {
        static char const *const names[] = { "vertex", 0 };
        return names;
    }

    void initialize()
    {
        QSGMaterialShader::initialize();
        m_id_matrix = program()->uniformLocation("matrix");
        m_id_opacity = program()->uniformLocation("opacity");
        m_id_color = program()->uniformLocation("color");
        m_id_data = program()->uniformLocation("image");
        m_id_width = program()->uniformLocation("width");
        m_id_height = program()->uniformLocation("height");
        m_id_amplitude = program()->uniformLocation("amplitude");
        m_id_offset = program()->uniformLocation("offset");
        m_id_p1 = program()->uniformLocation("p1");
        m_id_p2 = program()->uniformLocation("p2");
    }

    void activate() {
    }

    void updateState(const RenderState &state, QSGMaterial *newMaterial, QSGMaterial *)
    {
        Q_ASSERT(program()->isLinked());
        SlicePlotMaterial* material = static_cast<SlicePlotMaterial*>(newMaterial);

        if (state.isMatrixDirty())
            program()->setUniformValue(m_id_matrix, state.combinedMatrix());
        if (state.isOpacityDirty())
            program()->setUniformValue(m_id_opacity, state.opacity());

        // bind material parameters
        program()->setUniformValue(m_id_width, float(material->m_width));
        program()->setUniformValue(m_id_height, float(material->m_height));
        program()->setUniformValue(m_id_amplitude, float(material->m_amplitude));
        program()->setUniformValue(m_id_offset, float(material->m_offset));
        program()->setUniformValue(m_id_p1, material->m_p1);
        program()->setUniformValue(m_id_p2, material->m_p2);
        program()->setUniformValue(m_id_color, material->m_color);

        // bind the material data texture
        program()->setUniformValue(m_id_data, 0);
        material->m_texture_data->bind();
    }

    void deactivate() {
    }

private:
    int m_id_matrix;
    int m_id_opacity;
    int m_id_color;
    int m_id_width;
    int m_id_height;
    int m_id_data;
    int m_id_amplitude;
    int m_id_offset;
    int m_id_p1;
    int m_id_p2;
};


inline QSGMaterialShader* SlicePlotMaterial::createShader() const { return new SlicePlotShader; }

// ----------------------------------------------------------------------------


SlicePlot::SlicePlot(QQuickItem *parent) :
    DataClient(parent),
    m_min_value(0.), m_max_value(1.), m_num_segments(20),
    m_p1(0., 0.), m_p2(1., 1.),
    m_color(Qt::red)
{
    setFlag(QQuickItem::ItemHasContents);
    setClip(true);
}

SlicePlot::~SlicePlot()
{

}

void SlicePlot::setMinimumValue(double value)
{
    if (value == m_min_value) return;
    m_min_value = value;
    emit minimumValueChanged(value);
    update();
}

void SlicePlot::setMaximumValue(double value)
{
    if (value == m_max_value) return;
    m_max_value = value;
    emit maximumValueChanged(value);
    update();
}

void SlicePlot::setNumSegments(int n)
{
    if (n == m_num_segments) return;
    m_num_segments = n;
    m_new_geometry = true;
    emit numSegmentsChanged(n);
    update();
}

void SlicePlot::setP1(const QPointF &p)
{
    if (m_p1 == p) return;
    m_p1 = p;
    emit p1Changed(m_p1);
    update();
}

void SlicePlot::setP2(const QPointF &p)
{
    if (m_p2 == p) return;
    m_p2 = p;
    emit p2Changed(m_p2);
    update();
}

void SlicePlot::setColor(const QColor &color)
{
    if (m_color == color) return;
    m_color = color;
    emit colorChanged(m_color);
    update();
}

QSGNode *SlicePlot::updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *)
{
    QSGGeometryNode* n = static_cast<QSGGeometryNode*>(node);
    QSGGeometry *geometry;
    SlicePlotMaterial *material;

    if (n && !m_source) {
        // remove the node if there is no data source
        delete n;
        return nullptr;
    }

    if  (!n && m_source) {
        // create a node if there is a data source
        n = new QSGGeometryNode();
        // inintialize geometry
        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
        geometry->setDrawingMode(GL_LINE_STRIP);
        geometry->setLineWidth(1.);
        n->setGeometry(geometry);
        n->setFlag(QSGNode::OwnsGeometry);
        // initialize material
        material = new SlicePlotMaterial;
        material->m_texture_data = m_source->textureProvider()->texture();
        n->setMaterial(material);
        n->setFlag(QSGNode::OwnsMaterial);
    }

    if (!n) {
        // return null if there is nothing to be drawn
        return nullptr;
    }

    // ** graph node and data source can be considered valid from here on **

    geometry = n->geometry();
    material = static_cast<SlicePlotMaterial*>(n->material());
    QSGNode::DirtyState dirty_state = QSGNode::DirtyMaterial;

    // update material parameters
    double scale_val = 1. / (m_max_value - m_min_value);
    double off_val = -m_min_value;
    material->m_width = width();
    material->m_height = height();
    material->m_amplitude = - scale_val;
    material->m_offset = off_val - 1./scale_val;
    material->m_p1 = m_p1;
    material->m_p2 = m_p2;
    material->m_color = m_color;

    // update geometry if the number of segments changed
    if (geometry->vertexCount() != (m_num_segments+1)) {
        geometry->allocate(m_num_segments+1);
        float* data = static_cast<float*>(geometry->vertexData());
        for (int i = 0; i <= m_num_segments; ++i) {
            double f = i * (1./m_num_segments);
            data[2*i] = f;
        }
        dirty_state |= QSGNode::DirtyGeometry;
    }

    // check for data source change
    if (m_new_source) {
        material->m_texture_data = m_source->textureProvider()->texture();
        m_new_source = false;
    }

    // check if the image texture should be updated
    if (m_new_data) {
        static_cast<QSGDynamicTexture*>(material->m_texture_data)->updateTexture();
        m_new_data = false;
    }

    n->markDirty(dirty_state);
    return n;
}
