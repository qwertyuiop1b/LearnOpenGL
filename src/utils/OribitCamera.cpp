#include "OribitCamera.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include <cmath>


OribitCamera::OribitCamera(const glm::vec3& center, const glm::vec3& upVector, float radius, float minRadius, float azimuthAngle, float polarAngle)
    : mCenter(center)
    , mUpvector(upVector)
    , mRadius(radius)
    , mMinRadius(minRadius)
    , mAzimuthAngle(azimuthAngle)
    , mPolarAngle(polarAngle)
{

}


void OribitCamera::rotateAzimuth(const float radians) {
    mAzimuthAngle += radians;

    const auto fullCircle = 2.0f * glm::pi<float>();
    mAzimuthAngle = fmodf(mAzimuthAngle, fullCircle);
    if (mAzimuthAngle < 0.f) {
        mAzimuthAngle += fullCircle;
    }
}

void OribitCamera::rotatePolar(const float radians) {
    mPolarAngle += radians;

    const auto polarCap = glm::pi<float>() / 2.f - 0.001f;
    if (mPolarAngle > polarCap) {
        mPolarAngle = polarCap;
    }

    if (mPolarAngle < -polarCap) {
        mPolarAngle = -polarCap;
    }
}

void OribitCamera::zoom(const float by) {
    mRadius -= by;

    if (mRadius < mMinRadius) {
        mRadius = mMinRadius;
    }
}

void OribitCamera::moveHorizontal(const float distance) {
    const auto position = getEye();
    const glm::vec3 viewVector = getNormalizedViewVector();
    const glm::vec3 strafeVector = glm::normalize(glm::cross(viewVector, mUpvector));
    mCenter += strafeVector * distance;
}

void OribitCamera::moveVertical(const float distance) {
    mCenter += mUpvector * distance;
}

glm::mat4 OribitCamera::getViewMatrix() const {
    const auto eye = getEye();
    return glm::lookAt(eye, eye + getNormalizedViewVector(), mUpvector);
}

glm::vec3 OribitCamera::getNormalizedViewVector() const {
    return glm::normalize(getViewPoint() - getEye());
}

glm::vec3 OribitCamera::getEye() const {
    const auto sineAzimuth = sin(mAzimuthAngle);
    const auto cosineAzimuth = cos(mAzimuthAngle);
    const auto sinePolar = sin(mPolarAngle);
    const auto cosinePolar = cos(mPolarAngle);

    const auto x = mCenter.x + mRadius * cosinePolar * cosineAzimuth;
    const auto y = mCenter.y + mRadius * sinePolar;
    const auto z = mCenter.z + mRadius * cosinePolar * sineAzimuth;

    return glm::vec3(x, y, z);
}

