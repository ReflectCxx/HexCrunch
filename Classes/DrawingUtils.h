#pragma once

#include "cocos2d.h"
#include "Constants.h"

namespace hex::ut
{
    static void draw_hexagon(cocos2d::DrawNode* drawNode, float radius,
                             const cocos2d::Color4F& color)
    {
        constexpr int NUM_VERTICES = HEX_6;

        cocos2d::Vec2 vertices[NUM_VERTICES];

        for (int i = 0; i < NUM_VERTICES; ++i) {
            const float angle = (60.f * i + 30.f) * M_PI / 180.f;
            vertices[i] = { radius * std::cos(angle), radius * std::sin(angle) };
        }

        drawNode->drawSolidPoly(vertices, NUM_VERTICES, color);
    }


    static void draw_hexagon(cocos2d::DrawNode* drawNode, float radius, 
                             const cocos2d::Color4F& color, float cornerRadius)
    {
        constexpr int NUM_CORNERS = HEX_6;
        constexpr int ARC_SEGMENTS = 5;

        constexpr float DEG_TO_RAD = M_PI / 180.f;
        constexpr float HALF_INTERIOR_ANGLE = 60.f * DEG_TO_RAD;

        cocos2d::Vec2 corners[NUM_CORNERS];

        // Pointy-top hexagon.
        for (int i = 0; i < NUM_CORNERS; ++i) {
            const float angle = (60.f * i + 30.f) * DEG_TO_RAD;
            corners[i] = { radius * std::cos(angle), radius * std::sin(angle) };
        }

        // Prevent rounding from consuming too much of an edge.
        const float maxCornerRadius = radius * std::sin(HALF_INTERIOR_ANGLE);
        cornerRadius = std::min(cornerRadius, maxCornerRadius);

        std::vector<cocos2d::Vec2> vertices;
        vertices.reserve(NUM_CORNERS * (ARC_SEGMENTS + 1));

        for (int i = 0; i < NUM_CORNERS; ++i)
        {
            const auto prev = corners[(i + NUM_CORNERS - 1) % NUM_CORNERS];
            const auto curr = corners[i];
            const auto next = corners[(i + 1) % NUM_CORNERS];

            const cocos2d::Vec2 dirToPrev = (prev - curr).getNormalized();
            const cocos2d::Vec2 dirToNext = (next - curr).getNormalized();

            // Distance from the sharp vertex to each tangent point.
            const float tangentDistance = cornerRadius / std::tan(HALF_INTERIOR_ANGLE);
            const cocos2d::Vec2 tangentA = curr + dirToPrev * tangentDistance;
            const cocos2d::Vec2 tangentB = curr + dirToNext * tangentDistance;

            // Arc center lies along the inward angle bisector.
            const cocos2d::Vec2 inward = (-curr).getNormalized();
            const float centerDistance = cornerRadius / std::sin(HALF_INTERIOR_ANGLE);

            const cocos2d::Vec2 arcCenter = curr + inward * centerDistance;

            float startAngle = std::atan2(tangentA.y - arcCenter.y, tangentA.x - arcCenter.x);
            float endAngle = std::atan2(tangentB.y - arcCenter.y, tangentB.x - arcCenter.x);

            while (endAngle < startAngle) {
                endAngle += 2.f * M_PI;
            }

            for (int j = 0; j <= ARC_SEGMENTS; ++j) {
                const float t = static_cast<float>(j) / ARC_SEGMENTS;
                const float angle = startAngle + (endAngle - startAngle) * t;
                vertices.emplace_back(arcCenter.x + cornerRadius * std::cos(angle),
                    arcCenter.y + cornerRadius * std::sin(angle));
            }
        }
        drawNode->drawSolidPoly(vertices.data(), static_cast<int>(vertices.size()), color);
    }

    //Runs on schedular, not on any action.
    static void run_hex_bounce(cocos2d::Node* pNode, float pRadius, std::function<void(float)> pOnBounce)
    {
        constexpr float SPEED = 110.f;
        constexpr auto KEY = "hex_bounce";

        if (!pNode || pRadius <= 0.0f)
            return;

        const float apothem = pRadius * SQRT_3 * 0.5f;
        const std::array<cocos2d::Vec2, 6> normals = {
            cocos2d::Vec2{  1.0f,  0.0f },
            cocos2d::Vec2{  0.5f,  SQRT_3 * 0.5f },
            cocos2d::Vec2{ -0.5f,  SQRT_3 * 0.5f },
            cocos2d::Vec2{ -1.0f,  0.0f },
            cocos2d::Vec2{ -0.5f, -SQRT_3 * 0.5f },
            cocos2d::Vec2{  0.5f, -SQRT_3 * 0.5f }
        };

        const float angle = cocos2d::RandomHelper::random_real(0.0f, 2.0f * float(M_PI));
        cocos2d::Vec2 velocity{ std::cos(angle), std::sin(angle) };
        velocity.normalize();
        velocity *= SPEED;

        pNode->unschedule(KEY);
        pNode->schedule([pNode, velocity, normals, apothem, onBounce = std::move(pOnBounce)](float dt) mutable
        {
            cocos2d::Vec2 position = pNode->getPosition();
            position += velocity * dt;
            for (const cocos2d::Vec2& normal : normals)
            {
                const float distance = position.dot(normal) - apothem;
                if (distance > 0.0f) {

                    position -= normal * distance;
                    const float velocityAlongNormal = velocity.dot(normal);
                    if (velocityAlongNormal > 0.0f) {

                        velocity -= 2.0f * velocityAlongNormal * normal;
                        if (onBounce) {
                            const auto normalAngle = std::atan2(normal.y, normal.x);
                            onBounce(CC_RADIANS_TO_DEGREES(normalAngle));
                        }
                    }
                }
            }
            pNode->setPosition(position);
        }, KEY);
    }
}


namespace hex::ut
{
    static void draw_capsule(cocos2d::DrawNode* pNode, const cocos2d::Size& pSize,
                             const cocos2d::Color4F& pColor, const cocos2d::Vec2& pOrigin, float pAngle)
    {
        using namespace cocos2d;

        if (!pNode ||
            pSize.width <= 0.0f ||
            pSize.height <= 0.0f)
        {
            return;
        }

        if (pSize.width < pSize.height)
            return;

        constexpr int ARC_SEGMENTS = 30;

        const float radius = pSize.height * 0.5f;
        const float rectWidth = pSize.width - pSize.height;
        const float halfRectWidth = rectWidth * 0.5f;

        // cocos2d rotation convention:
        // positive angle = clockwise.
        const float angle =
            -pAngle * static_cast<float>(M_PI) / 180.0f;

        const float cosA = std::cos(angle);
        const float sinA = std::sin(angle);

        auto rotatePoint = [&](const Vec2& p) -> Vec2
            {
                const Vec2 d = p - pOrigin;

                return
                {
                    pOrigin.x + d.x * cosA - d.y * sinA,
                    pOrigin.y + d.x * sinA + d.y * cosA
                };
            };

        // ---------------------------------------------------------
        // Middle rectangle
        // ---------------------------------------------------------

        Vec2 rectVertices[4] =
        {
            rotatePoint({
                pOrigin.x - halfRectWidth,
                pOrigin.y + radius
            }),

            rotatePoint({
                pOrigin.x + halfRectWidth,
                pOrigin.y + radius
            }),

            rotatePoint({
                pOrigin.x + halfRectWidth,
                pOrigin.y - radius
            }),

            rotatePoint({
                pOrigin.x - halfRectWidth,
                pOrigin.y - radius
            })
        };

        pNode->drawPolygon(
            rectVertices,
            4,
            pColor,
            0.0f,
            pColor
        );

        // ---------------------------------------------------------
        // Left semicircle
        // ---------------------------------------------------------

        {
            const Vec2 center =
            {
                pOrigin.x - halfRectWidth,
                pOrigin.y
            };

            std::vector<Vec2> vertices;
            vertices.reserve(ARC_SEGMENTS + 2);

            vertices.push_back(
                rotatePoint(center)
            );

            // 90° -> 270°
            for (int i = 0; i <= ARC_SEGMENTS; ++i)
            {
                const float t =
                    static_cast<float>(i) /
                    static_cast<float>(ARC_SEGMENTS);

                const float arcAngle =
                    static_cast<float>(
                        (90.0 + 180.0 * t) *
                        M_PI / 180.0
                        );

                const Vec2 point =
                {
                    center.x +
                        std::cos(arcAngle) * radius,

                    center.y +
                        std::sin(arcAngle) * radius
                };

                vertices.push_back(
                    rotatePoint(point)
                );
            }

            pNode->drawPolygon(
                vertices.data(),
                static_cast<int>(vertices.size()),
                pColor,
                0.0f,
                pColor
            );
        }

        // ---------------------------------------------------------
        // Right semicircle
        // ---------------------------------------------------------

        {
            const Vec2 center =
            {
                pOrigin.x + halfRectWidth,
                pOrigin.y
            };

            std::vector<Vec2> vertices;
            vertices.reserve(ARC_SEGMENTS + 2);

            vertices.push_back(
                rotatePoint(center)
            );

            // -90° -> 90°
            for (int i = 0; i <= ARC_SEGMENTS; ++i)
            {
                const float t =
                    static_cast<float>(i) /
                    static_cast<float>(ARC_SEGMENTS);

                const float arcAngle =
                    static_cast<float>(
                        (-90.0 + 180.0 * t) *
                        M_PI / 180.0
                        );

                const Vec2 point =
                {
                    center.x +
                        std::cos(arcAngle) * radius,

                    center.y +
                        std::sin(arcAngle) * radius
                };

                vertices.push_back(
                    rotatePoint(point)
                );
            }

            pNode->drawPolygon(
                vertices.data(),
                static_cast<int>(vertices.size()),
                pColor,
                0.0f,
                pColor
            );
        }
    }
}