import esphome.codegen as cg
from esphome.components import light
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_OUTPUT_ID

from . import CONF_NEC_FAN_LIGHT_ID, NecFanLight, nec_fan_light_ns

DEPENDENCIES = ["nec_fan_light"]

NecLight = nec_fan_light_ns.class_("NecLight", light.LightOutput, cg.Component)

CONFIG_SCHEMA = light.BINARY_LIGHT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(NecLight),
        cv.GenerateID(CONF_NEC_FAN_LIGHT_ID): cv.use_id(NecFanLight),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_NEC_FAN_LIGHT_ID])
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID], parent)
    await cg.register_component(var, config)
    await light.register_light(var, config)
