import esphome.codegen as cg
from esphome.components import fan
import esphome.config_validation as cv
from esphome.const import CONF_ID

from . import CONF_NEC_FAN_LIGHT_ID, NecFanLight, nec_fan_light_ns

DEPENDENCIES = ["nec_fan_light"]

NecFan = nec_fan_light_ns.class_("NecFan", cg.Component, fan.Fan)

CONFIG_SCHEMA = fan.fan_schema(NecFan).extend(
    {
        cv.GenerateID(CONF_NEC_FAN_LIGHT_ID): cv.use_id(NecFanLight),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_NEC_FAN_LIGHT_ID])
    var = cg.new_Pvariable(config[CONF_ID], parent)
    await cg.register_component(var, config)
    await fan.register_fan(var, config)
