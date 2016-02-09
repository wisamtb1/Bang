#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <string>
#include <sstream>
#include <ostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

#include "Part.h"
#include "IToString.h"

class Transform : public Part, public IToString
{
public:
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;

    Transform();
    void GetMatrix(glm::mat4 &m) const;

    const std::string ToString() const override;
};

#endif // TRANSFORM_H
