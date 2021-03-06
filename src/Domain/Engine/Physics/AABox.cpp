#include "AABox.h"

#include "Debug.h"

#include "Rect.h"
#include "Camera.h"
#include "Sphere.h"

AABox AABox::Empty = AABox(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

AABox::AABox()
{
}

AABox::AABox(float minx, float maxx,
             float miny, float maxy,
             float minz, float maxz) :
    AABox(Vector3(minx, miny, minz),
          Vector3(maxx, maxy, maxz))
{
}

AABox::AABox(const Vector3 &min, const Vector3 &max)
{
    m_minv = min;
    m_maxv = max;
}

AABox::AABox(const AABox &b)
{
    m_minv = b.m_minv;
    m_maxv = b.m_maxv;
}

void AABox::SetMin(const Vector3 &bMin)
{
    m_minv = bMin;
}

void AABox::SetMax(const Vector3 &bMax)
{
    m_maxv = bMax;
}

const Vector3& AABox::GetMin() const
{
    return m_minv;
}

const Vector3& AABox::GetMax() const
{
    return m_maxv;
}

Vector3 AABox::GetDiagonal() const
{
    return m_maxv - m_minv;
}

float AABox::GetWidth() const
{
    return (m_maxv.x - m_minv.x);
}

float AABox::GetHeight() const
{
    return (m_maxv.y - m_minv.y);
}

float AABox::GetDepth() const
{
    return (m_maxv.z - m_minv.z);
}

Vector3 AABox::GetCenter() const
{
    return (m_minv + m_maxv) / 2.0f;
}

Vector3 AABox::GetDimensions() const
{
    return Vector3(GetWidth(), GetHeight(), GetDepth());
}

float AABox::GetArea() const
{
    float w = GetWidth();
    float h = GetHeight();
    float d = GetDepth();
    return w * h * 2 +
           w * d * 2 +
           h * d * 2;
}

float AABox::GetVolume() const
{
    float w = GetWidth();
    float h = GetHeight();
    float d = GetDepth();
    return w * h * d;
}

AABox AABox::Union(const AABox &b1, const AABox &b2)
{
    if (b1 == AABox::Empty) { return b2; }
    if (b2 == AABox::Empty) { return b1; }
    return
    AABox(glm::min(b1.m_minv.x, b2.m_minv.x), glm::max(b1.m_maxv.x, b2.m_maxv.x),
          glm::min(b1.m_minv.y, b2.m_minv.y), glm::max(b1.m_maxv.y, b2.m_maxv.y),
          glm::min(b1.m_minv.z, b2.m_minv.z), glm::max(b1.m_maxv.z, b2.m_maxv.z));
}

void AABox::FillFromPositions(const Array<Vector3> &positions)
{
    ASSERT(!positions.Empty());
    m_minv = m_maxv = positions[0];
    for (const Vector3 &v : positions)
    {
        m_minv.x = std::min(m_minv.x, v.x);
        m_maxv.x = std::max(m_maxv.x, v.x);

        m_minv.y = std::min(m_minv.y, v.y);
        m_maxv.y = std::max(m_maxv.y, v.y);

        m_minv.z = std::min(m_minv.z, v.z);
        m_maxv.z = std::max(m_maxv.z, v.z);
    }
}

AABox AABox::FromSphere(const Sphere &sphere)
{
    AABox b;
    b.FillFromPositions(sphere.GetPoints());
    return b;
}

Array<Vector3> AABox::GetPoints() const
{
    Vector3 center  = GetCenter();
    Vector3 extents = GetDimensions() / 2.0f;
    Vector3 p1 = center + extents * Vector3(-1, -1, -1);
    Vector3 p2 = center + extents * Vector3(-1, -1,  1);
    Vector3 p3 = center + extents * Vector3(-1,  1, -1);
    Vector3 p4 = center + extents * Vector3(-1,  1,  1);
    Vector3 p5 = center + extents * Vector3( 1, -1, -1);
    Vector3 p6 = center + extents * Vector3( 1, -1,  1);
    Vector3 p7 = center + extents * Vector3( 1,  1, -1);
    Vector3 p8 = center + extents * Vector3( 1,  1,  1);
    return {p1, p2, p3, p4, p5, p6, p7, p8};
}

Rect AABox::GetAABoundingScreenRect(Camera *cam) const
{
    Array<Vector3> boxPoints = (*this).GetPoints();
    List<Vector2> screenPoints;

    // TODO: Treat properly points outside the screen
    // (now we just return the full screen rect if 1 or more points are outside)
    for (const Vector3 &p : boxPoints)
    {
        Vector2 screenP = cam->WorldToScreenNDCPoint(p);
        screenPoints.PushBack(screenP);
    }

    Rect boundingRect = Rect::GetBoundingRectFromPositions(screenPoints);
    return boundingRect;
}

AABox operator*(const Matrix4 &m, const AABox &b)
{
    Array<Vector3> points = b.GetPoints();
    Array<Vector3> newTransformedBoxPoints =
        {
            (m * Vector4(points[0], 1)).xyz(),
            (m * Vector4(points[1], 1)).xyz(),
            (m * Vector4(points[2], 1)).xyz(),
            (m * Vector4(points[3], 1)).xyz(),
            (m * Vector4(points[4], 1)).xyz(),
            (m * Vector4(points[5], 1)).xyz(),
            (m * Vector4(points[6], 1)).xyz(),
            (m * Vector4(points[7], 1)).xyz()
        };

    // Debug_Log(newTransformedBoxPoints);

    AABox br;
    br.FillFromPositions(newTransformedBoxPoints);
    return br;
}



const String AABox::ToString() const
{
    std::ostringstream oss;
    oss << "Box: [" << std::endl <<
           "  min: " << m_minv << std::endl <<
           "  max" << m_maxv << std::endl <<
           "]" << std::endl;

    return oss.str();
}


bool operator==(const AABox &b1, const AABox &b2)
{
    return b1.GetMin() == b2.GetMin() &&
           b1.GetMax() == b2.GetMax();
}
