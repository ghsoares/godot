#include "register_types.h"

#include "core/class_db.h"
#include "collision_object_spatial_2d.h"
#include "collision_shape_spatial_2d.h"
#include "physics_body_spatial_2d.h"
#include "collision_shape_spatial_2d_gizmo_plugin.h"

void register_collision_spatial_2d_types() {
	ClassDB::register_virtual_class<CollisionObjectSpatial2D>();
	ClassDB::register_virtual_class<PhysicsBodySpatial2D>();
	
	ClassDB::register_class<CollisionShapeSpatial2D>();

	ClassDB::register_class<StaticBodySpatial2D>();
	ClassDB::register_class<RigidBodySpatial2D>();
	ClassDB::register_class<KinematicBodySpatial2D>();
	ClassDB::register_class<KinematicCollisionSpatial2D>();

	#ifdef TOOLS_ENABLED
		EditorPlugins::add_by_type<EditorPluginCollisionShapeSpatial2D>();
	#endif
}

void unregister_collision_spatial_2d_types() {

}