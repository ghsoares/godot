#include "collision_shape_spatial_2d.h"

#include "collision_object_spatial_2d.h"
#include "core/engine.h"
//#include "scene/2d/area_2d.h"
#include "scene/resources/concave_polygon_shape_2d.h"
#include "scene/resources/convex_polygon_shape_2d.h"

void CollisionShapeSpatial2D::_shape_changed() {
	update_gizmo();
}

void CollisionShapeSpatial2D::_update_in_shape_owner(bool p_xform_only) {
	parent->shape_owner_set_transform(owner_id, transform_to_2d(get_transform()));
	if (p_xform_only) {
		return;
	}
	parent->shape_owner_set_disabled(owner_id, disabled);
	parent->shape_owner_set_one_way_collision(owner_id, one_way_collision);
	parent->shape_owner_set_one_way_collision_margin(owner_id, one_way_collision_margin);
}

void CollisionShapeSpatial2D::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_PARENTED: {
			parent = Object::cast_to<CollisionObjectSpatial2D>(get_parent());
			if (parent) {
				owner_id = parent->create_shape_owner(this);
				if (shape.is_valid()) {
					parent->shape_owner_add_shape(owner_id, shape);
				}
				_update_in_shape_owner();
			}
		} break;
		case NOTIFICATION_ENTER_TREE: {
			if (parent) {
				_update_in_shape_owner();
			}

		} break;
		case NOTIFICATION_LOCAL_TRANSFORM_CHANGED: {
			if (parent) {
				_update_in_shape_owner(true);
			}
		} break;
		case NOTIFICATION_TRANSFORM_CHANGED: {
			update_gizmo();
		} break;
		case NOTIFICATION_UNPARENTED: {
			if (parent) {
				parent->remove_shape_owner(owner_id);
			}
			owner_id = 0;
			parent = nullptr;

		} break;
	}
}

void CollisionShapeSpatial2D::set_shape(const Ref<Shape2D> &p_shape) {
	if (p_shape == shape) {
		return;
	}
	if (shape.is_valid()) {
		shape->disconnect("changed", this, "_shape_changed");
	}
	shape = p_shape;
	update_gizmo();
	if (parent) {
		parent->shape_owner_clear_shapes(owner_id);
		if (shape.is_valid()) {
			parent->shape_owner_add_shape(owner_id, shape);
		}
		_update_in_shape_owner();
	}

	if (shape.is_valid()) {
		shape->connect("changed", this, "_shape_changed");
	}

	update_configuration_warning();
}

Ref<Shape2D> CollisionShapeSpatial2D::get_shape() const {
	return shape;
}

bool CollisionShapeSpatial2D::_edit_is_selected_on_click(const Point2 &p_point, double p_tolerance) const {
	if (!shape.is_valid()) {
		return false;
	}

	return shape->_edit_is_selected_on_click(p_point, p_tolerance);
}

String CollisionShapeSpatial2D::get_configuration_warning() const {
	String warning = Spatial::get_configuration_warning();

	if (!Object::cast_to<CollisionObjectSpatial2D>(get_parent())) {
		if (warning != String()) {
			warning += "\n\n";
		}
		warning += TTR("CollisionShapeSpatial2D only serves to provide a collision shape to a CollisionObjectSpatial2D derived node. Please only use it as a child of AreaSpatial2D, StaticBodySpatial2D, RigidBodySpatial2D, KinematicBodySpatial2D, etc. to give them a shape.");
	}

	if (!shape.is_valid()) {
		if (warning != String()) {
			warning += "\n\n";
		}
		warning += TTR("A shape must be provided for CollisionShapeSpatial2D to function. Please create a shape resource for it!");
	} else {
		Ref<ConvexPolygonShape2D> convex = shape;
		Ref<ConcavePolygonShape2D> concave = shape;
		if (convex.is_valid() || concave.is_valid()) {
			if (warning != String()) {
				warning += "\n\n";
			}
			warning += TTR("Polygon-based shapes are not meant be used nor edited directly through the CollisionShapeSpatial2D node. Please use the CollisionPolygon2D node instead.");
		}
	}
	/*if (one_way_collision && Object::cast_to<Area2D>(get_parent())) {
		warning += TTR("The One Way Collision property will be ignored when the parent is an Area2D.");
	}*/

	return warning;
}

void CollisionShapeSpatial2D::set_disabled(bool p_disabled) {
	disabled = p_disabled;
	update_gizmo();
	if (parent) {
		parent->shape_owner_set_disabled(owner_id, p_disabled);
	}
}

bool CollisionShapeSpatial2D::is_disabled() const {
	return disabled;
}

void CollisionShapeSpatial2D::set_one_way_collision(bool p_enable) {
	one_way_collision = p_enable;
	update_gizmo();
	if (parent) {
		parent->shape_owner_set_one_way_collision(owner_id, p_enable);
	}
	update_configuration_warning();
}

bool CollisionShapeSpatial2D::is_one_way_collision_enabled() const {
	return one_way_collision;
}

void CollisionShapeSpatial2D::set_one_way_collision_margin(float p_margin) {
	one_way_collision_margin = p_margin;
	if (parent) {
		parent->shape_owner_set_one_way_collision_margin(owner_id, one_way_collision_margin);
	}
}

float CollisionShapeSpatial2D::get_one_way_collision_margin() const {
	return one_way_collision_margin;
}

void CollisionShapeSpatial2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_shape", "shape"), &CollisionShapeSpatial2D::set_shape);
	ClassDB::bind_method(D_METHOD("get_shape"), &CollisionShapeSpatial2D::get_shape);
	ClassDB::bind_method(D_METHOD("set_disabled", "disabled"), &CollisionShapeSpatial2D::set_disabled);
	ClassDB::bind_method(D_METHOD("is_disabled"), &CollisionShapeSpatial2D::is_disabled);
	ClassDB::bind_method(D_METHOD("set_one_way_collision", "enabled"), &CollisionShapeSpatial2D::set_one_way_collision);
	ClassDB::bind_method(D_METHOD("is_one_way_collision_enabled"), &CollisionShapeSpatial2D::is_one_way_collision_enabled);
	ClassDB::bind_method(D_METHOD("set_one_way_collision_margin", "margin"), &CollisionShapeSpatial2D::set_one_way_collision_margin);
	ClassDB::bind_method(D_METHOD("get_one_way_collision_margin"), &CollisionShapeSpatial2D::get_one_way_collision_margin);
	ClassDB::bind_method(D_METHOD("_shape_changed"), &CollisionShapeSpatial2D::_shape_changed);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "shape", PROPERTY_HINT_RESOURCE_TYPE, "Shape2D"), "set_shape", "get_shape");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "disabled"), "set_disabled", "is_disabled");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "one_way_collision"), "set_one_way_collision", "is_one_way_collision_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "one_way_collision_margin", PROPERTY_HINT_RANGE, "0,128,0.1"), "set_one_way_collision_margin", "get_one_way_collision_margin");
}

CollisionShapeSpatial2D::CollisionShapeSpatial2D() {
	rect = Rect2(-Point2(10, 10), Point2(20, 20));
	set_notify_local_transform(true);
	owner_id = 0;
	parent = nullptr;
	disabled = false;
	one_way_collision = false;
	one_way_collision_margin = 1.0;
}

