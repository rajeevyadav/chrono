// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2016 projectchrono.org
// All right reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================
// Authors: Radu Serban
// =============================================================================

#pragma once

#include <vector>

#include "chrono/physics/ChBody.h"

#include "chrono_distributed/ChApiDistributed.h"
#include "chrono_parallel/collision/ChCollisionModelParallel.h"
#include "chrono_parallel/physics/ChSystemParallel.h"

namespace chrono {

/// Utility class for specifying a collision boundary composed of multiple semi-planes.
class CH_DISTR_API ChBoundary : public ChSystem::CustomCollisionCallback {
  public:
    ChBoundary(std::shared_ptr<ChBody> body);
    ~ChBoundary() {}

    /// Add a collision plane with finite extent and return the plane ID.
    /// The plane normal is in the Z-direction of the given frame.
    void AddPlane(const ChFrame<>& frame,     ///< plane reference frame, relative to associated body
                  const ChVector2<>& lengths  ///< X-Y extent
    );

    /// Update all collision planes.  
    /// This function should be called if the position of the associated body is modified.
    void Update();

    /// Override the position of the specified collision plane.
    void UpdatePlane(size_t id,              ///< plane index
                     const ChFrame<>& frame  ///< plane reference frame, relative to associated body
    );

    /// Override the X-Y extent of the specified collision plane.
    void UpdatePlane(size_t id,                  ///< plane index
                     const ChVector2<>& lengths  ///< X-Y extent
    );

    /// Add visualization for all existing planes.
    void AddVisualization(double thickness);

    /// Return the current number of collisions.
    int GetNContacts() const { return m_crt_count; }

    /// Return the associated body.
    std::shared_ptr<ChBody> GetBody() const { return m_body; }

  private:
    struct Plane {
        Plane(const ChFrame<>& frame_loc, const ChFrame<>& frame, const ChVector2<>& lengths);
        ChFrame<> m_frame_loc;  ///< plane coordinate frame, relative to associated body
        ChFrame<> m_frame;      ///< plane coordinate frame, expressed in global (Z axis defines normal)
        ChVector2<> m_hlen;     ///< half-extents in X and Y directions
        ChVector<> m_normal;    ///< cached plane normal, expressed in global
    };

    virtual void OnCustomCollision(ChSystem* system) override;

    void CheckSphere(std::shared_ptr<collision::ChCollisionModel> model, const ChVector<>& center, double radius);
    void CheckSpherePlane(std::shared_ptr<collision::ChCollisionModel> model,
                          const ChVector<>& center,
                          double radius,
                          const Plane& plane);

    void CheckBox(std::shared_ptr<collision::ChCollisionModel> model, const ChFrame<>& frame, const ChVector<>& size);
    void CheckBoxPlane(std::shared_ptr<collision::ChCollisionModel> model,
                       const ChFrame<>& frame,
                       const ChVector<>& size,
                       const Plane& plane);

    std::shared_ptr<ChBody> m_body;  ///< body to which boundary is attached
    std::vector<Plane> m_planes;     ///< list of boundary planes

    int m_crt_count;
};

}  // end namespace chrono