#include "collision.hpp"

bool rayTriangleCollision(
    glm::vec3 R0,
    glm::vec3 R1,
    glm::vec3 T0,
    glm::vec3 T1,
    glm::vec3 T2
)
{
    if ( glm::determinant(glm::mat3( R1-R0, T0-R1, T1-T0 )) >= 0.0 ) 
    {
        return  ( glm::determinant(glm::mat3( R1-R0, T1-R1, T2-T1 )) >= 0.0 )
            and ( glm::determinant(glm::mat3( R1-R0, T2-R1, T0-T2 )) >= 0.0 );
    }
    else
    {
        return  ( glm::determinant(glm::mat3( R1-R0, T1-R1, T2-T1 )) <= 0.0 )
            and ( glm::determinant(glm::mat3( R1-R0, T2-R1, T0-T2 )) <= 0.0 );
    }
}
