#include "collision_shape_spatial_2d_gizmo_plugin.h"
#include "collision_object_spatial_2d.h"
#include "collision_shape_spatial_2d.h"
#include "core/engine.h"
//#include "scene/2d/area_2d.h"
#include "scene/resources/capsule_shape_2d.h"
#include "scene/resources/circle_shape_2d.h"
#include "scene/resources/concave_polygon_shape_2d.h"
#include "scene/resources/convex_polygon_shape_2d.h"
#include "scene/resources/line_shape_2d.h"
#include "scene/resources/rectangle_shape_2d.h"
#include "scene/resources/segment_shape_2d.h"

void xform_2d_points(Transform tr, Vector<Vector3> vec) {
	Vector3 *vec_ptr	= vec.ptrw();

	int count = vec.size();
	for (int i = 0; i < count; i++) {
		vec_ptr[i] = xform_2d(tr, vec_ptr[i]);
	}
}

CollisionShapeSpatial2DGizmoPlugin::CollisionShapeSpatial2DGizmoPlugin() {
	Color shape_color = EDITOR_GET("editors/3d_gizmos/gizmo_colors/shape");
	
	Ref<SpatialMaterial> shape_material = Ref<SpatialMaterial>(memnew(SpatialMaterial));

	shape_material->set_flag(SpatialMaterial::FLAG_UNSHADED, true);
	shape_material->set_flag(SpatialMaterial::FLAG_ALBEDO_FROM_VERTEX_COLOR, true);
	shape_material->set_flag(SpatialMaterial::FLAG_SRGB_VERTEX_COLOR, true);
	shape_material->set_flag(SpatialMaterial::FLAG_DISABLE_DEPTH_TEST, true);

	shape_material->set_line_width(3);
	shape_material->set_albedo(shape_color);
	
	add_material("shape", shape_material);
}

bool CollisionShapeSpatial2DGizmoPlugin::has_gizmo(Spatial *p_spatial) {
	return Object::cast_to<CollisionShapeSpatial2D>(p_spatial);
}

String CollisionShapeSpatial2DGizmoPlugin::get_name() const {
	return "CollisionShapeSpatial2D";
}

bool CollisionShapeSpatial2DGizmoPlugin::is_selectable_when_hidden() const {
	return true;
}

void CollisionShapeSpatial2DGizmoPlugin::redraw(EditorSpatialGizmo *p_gizmo) {
	CollisionShapeSpatial2D *cs = Object::cast_to<CollisionShapeSpatial2D>(p_gizmo->get_spatial_node());

	p_gizmo->clear();

	Ref<Shape2D> s = cs->get_shape();
	if (s.is_null()) {
		return;
	}

	Transform tr 	= cs->get_global_transform();

	Ref<Material> shape_material = get_material("shape", p_gizmo);
	
	if (Object::cast_to<RectangleShape2D>(*s)) {
		Ref<RectangleShape2D> rs = s;

		Vector2 e = rs->get_extents();

		Vector<Vector3> lines;

		lines.push_back(Vector3(-e.x, e.y, 0.0));
		lines.push_back(Vector3(e.x, e.y, 0.0));

		lines.push_back(Vector3(e.x, e.y, 0.0));
		lines.push_back(Vector3(e.x, -e.y, 0.0));

		lines.push_back(Vector3(e.x, -e.y, 0.0));
		lines.push_back(Vector3(-e.x, -e.y, 0.0));

		lines.push_back(Vector3(-e.x, -e.y, 0.0));
		lines.push_back(Vector3(-e.x, e.y, 0.0));

		xform_2d_points(tr, lines);

		p_gizmo->add_lines(lines, shape_material);
		p_gizmo->add_collision_segments(lines);
	}

	if (Object::cast_to<SegmentShape2D>(*s)) {
		Ref<SegmentShape2D> ss = s;

		Vector2 a = ss->get_a();
		Vector2 b = ss->get_b();

		Vector<Vector3> lines;

		lines.push_back(Vector3(a.x, a.y, 0.0));
		lines.push_back(Vector3(b.x, b.y, 0.0));

		xform_2d_points(tr, lines);

		p_gizmo->add_lines(lines, shape_material);
		p_gizmo->add_collision_segments(lines);
	}

	if (Object::cast_to<CircleShape2D>(*s)) {
		Ref<CircleShape2D> cs = s;

		float r = cs->get_radius();

		Vector<Vector3> lines;
		Vector<Vector3> col_lines;

		int res = 90;

		for (int i = 0; i <= res; i++) {
			float ra = i * Math_PI * 2.0 / res;
			float rb = (i + 1) * Math_PI * 2.0 / res;
			Point2 a = Vector2(Math::cos(ra), Math::sin(ra)) * r;
			Point2 b = Vector2(Math::cos(rb), Math::sin(rb)) * r;

			lines.push_back(Vector3(a.x, a.y, 0.0));
			lines.push_back(Vector3(b.x, b.y, 0.0));
		}

		res = 64;

		for (int i = 0; i <= res; i++) {
			float ra = i * Math_PI * 2.0 / res;
			float rb = (i + 1) * Math_PI * 2.0 / res;
			Point2 a = Vector2(Math::cos(ra), Math::sin(ra)) * r;
			Point2 b = Vector2(Math::cos(rb), Math::sin(rb)) * r;

			col_lines.push_back(Vector3(a.x, a.y, 0.0));
			col_lines.push_back(Vector3(b.x, b.y, 0.0));
		}

		xform_2d_points(tr, lines);
		xform_2d_points(tr, col_lines);

		p_gizmo->add_lines(lines, shape_material);
		p_gizmo->add_collision_segments(col_lines);
	}

	if (Object::cast_to<CapsuleShape2D>(*s)) {
		Ref<CapsuleShape2D> cs = s;

		float h = cs->get_height();
		float r = cs->get_radius();

		Vector<Vector3> lines;
		Vector<Vector3> col_lines;

		int res = 90;
		int i = 0;

		for (i = 0; i <= res / 2; i++) {
			float ra = i * Math_PI * 2.0 / res;
			float rb = (i + 1) * Math_PI * 2.0 / res;
			Point2 a = Vector2(Math::cos(ra), Math::sin(ra)) * r;
			Point2 b = Vector2(Math::cos(rb), Math::sin(rb)) * r;

			lines.push_back(Vector3(a.x, a.y + h * 0.5, 0.0));
			lines.push_back(Vector3(b.x, b.y + h * 0.5, 0.0));
		}

		lines.push_back(Vector3(r, h * 0.5, 0.0));
		lines.push_back(Vector3(r, -h * 0.5, 0.0));

		for (i = res / 2; i <= res; i++) {
			float ra = i * Math_PI * 2.0 / res;
			float rb = (i + 1) * Math_PI * 2.0 / res;
			Point2 a = Vector2(Math::cos(ra), Math::sin(ra)) * r;
			Point2 b = Vector2(Math::cos(rb), Math::sin(rb)) * r;

			lines.push_back(Vector3(a.x, a.y - h * 0.5, 0.0));
			lines.push_back(Vector3(b.x, b.y - h * 0.5, 0.0));
		}

		lines.push_back(Vector3(-r, -h * 0.5, 0.0));
		lines.push_back(Vector3(-r, h * 0.5, 0.0));

		res = 64;

		for (i = 0; i <= res / 2; i++) {
			float ra = i * Math_PI * 2.0 / res;
			float rb = (i + 1) * Math_PI * 2.0 / res;
			Point2 a = Vector2(Math::cos(ra), Math::sin(ra)) * r;
			Point2 b = Vector2(Math::cos(rb), Math::sin(rb)) * r;

			col_lines.push_back(Vector3(a.x, a.y + h * 0.5, 0.0));
			col_lines.push_back(Vector3(b.x, b.y + h * 0.5, 0.0));
		}

		col_lines.push_back(Vector3(r, h * 0.5, 0.0));
		col_lines.push_back(Vector3(r, -h * 0.5, 0.0));

		for (i = res / 2; i <= res; i++) {
			float ra = i * Math_PI * 2.0 / res;
			float rb = (i + 1) * Math_PI * 2.0 / res;
			Point2 a = Vector2(Math::cos(ra), Math::sin(ra)) * r;
			Point2 b = Vector2(Math::cos(rb), Math::sin(rb)) * r;

			col_lines.push_back(Vector3(a.x, a.y - h * 0.5, 0.0));
			col_lines.push_back(Vector3(b.x, b.y - h * 0.5, 0.0));
		}

		col_lines.push_back(Vector3(-r, -h * 0.5, 0.0));
		col_lines.push_back(Vector3(-r, h * 0.5, 0.0));

		xform_2d_points(tr, lines);
		xform_2d_points(tr, col_lines);

		p_gizmo->add_lines(lines, shape_material);
		p_gizmo->add_collision_segments(col_lines);
	}
}

EditorPluginCollisionShapeSpatial2D::EditorPluginCollisionShapeSpatial2D(EditorNode *p_editor) {
	Ref<CollisionShapeSpatial2DGizmoPlugin> gizmo_plugin = Ref<CollisionShapeSpatial2DGizmoPlugin>(memnew(CollisionShapeSpatial2DGizmoPlugin));
	SpatialEditor::get_singleton()->add_gizmo_plugin(gizmo_plugin);
}