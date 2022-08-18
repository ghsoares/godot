#ifndef COLLISION_SHAPE_SPATIAL_2D_GIZMO_PLUGIN
#define COLLISION_SHAPE_SPATIAL_2D_GIZMO_PLUGIN

#include "editor/editor_plugin.h"
#include "editor/spatial_editor_gizmos.h"

class CollisionShapeSpatial2DGizmoPlugin : public EditorSpatialGizmoPlugin {
	GDCLASS(CollisionShapeSpatial2DGizmoPlugin, EditorSpatialGizmoPlugin);

public:
	bool has_gizmo(Spatial *p_spatial);
	String get_name() const;
	bool is_selectable_when_hidden() const;
	void redraw(EditorSpatialGizmo *p_gizmo);

	CollisionShapeSpatial2DGizmoPlugin();
};

class EditorPluginCollisionShapeSpatial2D : public EditorPlugin {
	GDCLASS(EditorPluginCollisionShapeSpatial2D, EditorPlugin);

public:
	EditorPluginCollisionShapeSpatial2D(EditorNode *p_editor);
};

#endif // COLLISION_SHAPE_SPATIAL_2D_GIZMO_PLUGIN