bl_info = {
    "name": "Framework Level Exporter (.level.xml)",
    "blender": (2, 80, 0),
    "category": "Import-Export",
}

import bpy

from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty, BoolProperty, FloatVectorProperty
from bpy.types import Operator, Text, Object


def process_script(script, object):
    string = script.as_string()

    for key, value in object.items():
        if type(value) == Object:
            string = string.replace("$" + key + "$", "@" + value.name)

    try:
        string = string.replace("$THIS$", "@" + object.name)
    except:
        pass

    return string


def print_matrix(name, matrix, file):
    letters = "xzyw"

    file.write("<%s>\n" % name)

    for x in range(4):
        file.write("<d%s " % letters[x])
        for y in range(4):
            file.write(' %s="%f"' % (letters[y], matrix[y][x]))
        file.write("/>\n")
    file.write("</" + str(name) + ">\n")


def write_option(key, value, file):
    tp = type(value)

    if tp in [Text]:
        return

    if tp == Object:
        file.write('<%s value="%s"/>\n' % (key, value.name))
        return

    if tp in [int, float, bool, str]:
        file.write('<%s value="%s"/>\n' % (key, str(value)))
        return

    letters = "xyzw"
    file.write("<%s " % key)
    for id in range(len(value)):
        file.write('%s="%s"' % (letters[id], str(value[id])))
    file.write("/>\n")


def write_script(script, file):
    file.write("<script content='%s'/>\n\n" % script)


def export_world(world, file):
    scripts = []
    for key, value in world.items():
        if type(value) == Text and key.count("script") > 0:
            scripts.append(process_script(value, world))
    for script in scripts:
        write_script(script, file)
    
    if "type" not in world.keys():
        return

    file.write('<%s name="%s" type="meta">\n' % (world["type"], world.name))
    
    for key, value in world.items():
        write_option(key, value, file)    

    file.write("</%s>\n\n" % world["type"])


def export_object(object, file):
    if "type" not in object.keys():
        return

    file.write('<%s name="%s">\n' % (object["type"], object.name))

    print_matrix("transform", object.matrix_world, file)

    scripts = []

    for key, value in object.items():
        write_option(key, value, file)
        if type(value) == Text and key.count("script") > 0:
            scripts.append(process_script(value, object))

    if object.name[0] == "[":
        source = object.name[1 : object.name.rfind("]")]
        write_option("source", source, file)

    if object.type == "LIGHT":
        power = object.data.energy / 10.0
        color = object.data.color
        file.write(
            """<color x="%f" y="%f" z="%f"/>\n"""
            % (color.r * power, color.g * power, color.b * power)
        )

    file.write('<blender_type value="%s"/>\n' % object.type)

    file.write("</%s>\n\n" % object["type"])

    for script in scripts:
        write_script(script, file)


def write_some_data(context, filepath, settings):
    print("exporting current scene as level %s..." % filepath)
    f = open(filepath, "w", encoding="utf-8")

    f.write("<!-- Blender %s www.blender.org -->\n\n" % bpy.app.version_string)

    f.write("<level>\n")

    scene = context.scene

    for obj in scene.objects:
        export_object(obj, f)

    export_world(scene.world, f)

    if settings["al"]:
        f.write('<ambient_light name="__WORLD_AMBIENT_LIGHT__">\n')
        write_option("color", scene.world.color, f)
        f.write("</ambient_light>\n")

    f.write("</level>\n")

    f.close()

    return {"FINISHED"}


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
        default=True,
    )

    def execute(self, context):
        return write_some_data(
            context,
            self.filepath,
            {"al": self.al_enabled},
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
