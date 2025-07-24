#ifndef OPENGL_ORIBIT_CAMERA
#define OPENGL_ORIBIT_CAMERA

#include "glm/fwd.hpp"
#include <glm/glm.hpp>

class OribitCamera {
public:

    OribitCamera(const glm::vec3& center, const glm::vec3& upVector, float radius, float minRadius, float azimuthAngle, float polarAngle);

    void rotateAzimuth(const float radians);

    void rotatePolar(const float radians);

    void zoom(const float by);

    void moveHorizontal(const float distance);
    void moveVertical(const float distance);

    glm::mat4 getViewMatrix() const;
    glm::vec3 getEye() const;
    glm::vec3 getViewPoint() const { return mCenter; };
    glm::vec3 getUpVector() const { return mUpvector; };
    glm::vec3 getNormalizedViewVector() const;
    float getAzimuthAngle() const { return mAzimuthAngle; }
    float getPolarAngle() const { return mPolarAngle; }
    float getRadius() const { return mRadius; }

private:
    glm::vec3 mCenter;
    glm::vec3 mUpvector;
    float mRadius;
    float mMinRadius;
    float mAzimuthAngle;
    float mPolarAngle;
};

#endif