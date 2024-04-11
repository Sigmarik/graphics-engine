bl_info = {
    "name": "Framework Level Exporter (.level.xml)",
    "blender": (2, 80, 0),
    "category": "Import-Export",
}

import bpy


def print_matrix(name, matrix, file):
    letters = "xyzw"

    file.write("<%s>\n" % name)

    for x in range(4):
        file.write("<d%s " % letters[x])
        for y in range(4):
            file.write(' %s="%g"' % (letters[y], matrix[y][x]))
        file.write("></d%s>\n" % letters[x])
    file.write("</" + str(name) + ">\n")


def write_option(key, value, file):
    tp = type(value)

    if tp in [int, float, bool, str]:
        file.write('<%s value="%s"></%s>\n' % (key, str(value), key))
        return

    letters = "xyzw"
    file.write("<%s " % key)
    for id in range(len(value)):
        file.write('%s="%s"' % (letters[id], str(value[id])))
    file.write("></%s>\n" % key)


def export_object(object, file):
    if "type" not in object.keys():
        return

    file.write('<%s name="%s"' % (object.data["type"], object.name))

    print_matrix("transform", object.matrix_world, file)

    for key, value in object.items():
        write_option(key, value, file)

    if object.name[0] == "[":
        source = object.name[1 : object.name.rfind("]")]
        write_option("source", source, file)

    if object.type == "LIGHT":
        power = object.data.energy / 10.0
        color = object.data.color
        file.write(
            """<color x="%g" y="%g" z="%g"></color>\n"""
            % (color.r * power, color.g * power, color.b * power)
        )

    file.write('<blender_type value="%s"></blender_type>\n' % object.type)

    file.write("</%s>\n\n" % object.data["type"])


def write_some_data(context, filepath, settings):
    print("exporting current scene as level %s..." % filepath)
    f = open(filepath, "w", encoding="utf-8")

    f.write("<level>\n")

    scene = context.scene

    for obj in scene.objects:
        export_object(obj, f)

    if settings["al"]:
        f.write('<ambient_light name="__WORLD_AMBIENT_LIGHT__">\n')
        write_option("color", settings["al_color"], f)
        f.write("</ambient_light>\n")

    f.write("</level>\n")

    f.close()

    return {"FINISHED"}


from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty, BoolProperty, FloatVectorProperty
from bpy.types import Operator


class ExportSomeData(Operator, ExportHelper):
    """XML-based level format used by the framework"""

    bl_idname = "level_exporter.export"
    bl_label = "Export Level"

    filename_ext = ".xml"

    filter_glob: StringProperty(
        default="*.level.xml",
        options={"HIDDEN"},
        maxlen=255,
    )

    al_enabled: BoolProperty(
        name="Add ambient light",
        description="Add ambient light object to the scene",
        default=False,
    )

    al_color: FloatVectorProperty(
        name="Ambient color",
        description="Ambient light color and intensity",
        default=[0.3, 0.3, 0.4],
        subtype="COLOR",
    )

    def execute(self, context):
        return write_some_data(
            context,
            self.filepath,
            {"al": self.al_enabled, "al_color": self.al_color},
        )


def menu_func_export(self, context):
    self.layout.operator(
        ExportSomeData.bl_idname, text="Game Level (.level.xml)"
    )


def register():
    bpy.utils.register_class(ExportSomeData)
    bpy.types.TOPBAR_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_class(ExportSomeData)
    bpy.types.TOPBAR_MT_file_export.remove(menu_func_export)


if __name__ == "__main__":
    register()
