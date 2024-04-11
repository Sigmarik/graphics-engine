bl_info = {
    "name": "Framework static mesh transformer",
    "blender": (2, 80, 0),
    "category": "Development",
}

import bpy


def main(context, source):
    object = context.object

    if object is None:
        return
    if object.type != "MESH":
        return

    object["type"] = "static_mesh"
    object["source"] = source


#    object.show_name = True;


from bpy.props import StringProperty


class SimpleOperator(bpy.types.Operator):
    """Transform the current object into framework-recognizable mesh"""

    bl_idname = "object.transform_to_framework_mesh"
    bl_label = "Transform to framework mesh"

    source: StringProperty(
        default="path_to_the_model.obj",
    )

    @classmethod
    def poll(cls, context):
        object = context.active_object

        if object is None:
            return False

        return object.type == "MESH"

    def invoke(self, context, event):
        wm = context.window_manager
        return wm.invoke_props_dialog(self)

    def execute(self, context):
        main(context, self.source)
        return {"FINISHED"}


def menu_func(self, context):
    self.layout.operator(SimpleOperator.bl_idname, text=SimpleOperator.bl_label)


# Register and add to the "object" menu (required to also use F3 search "Simple Object Operator" for quick access).
def register():
    bpy.utils.register_class(SimpleOperator)
    bpy.types.VIEW3D_MT_object.append(menu_func)


def unregister():
    bpy.utils.unregister_class(SimpleOperator)
    bpy.types.VIEW3D_MT_object.remove(menu_func)


if __name__ == "__main__":
    register()
