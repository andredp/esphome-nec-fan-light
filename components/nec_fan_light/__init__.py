import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import remote_base
from esphome.const import CONF_ID

CODEOWNERS = ["@andredp"]
DEPENDENCIES = ["remote_transmitter"]
AUTO_LOAD = ["remote_base"]
MULTI_CONF = True

nec_fan_light_ns = cg.esphome_ns.namespace("nec_fan_light")
NecFanLight = nec_fan_light_ns.class_(
    "NecFanLight", cg.Component, remote_base.RemoteTransmittable
)

CONF_NEC_FAN_LIGHT_ID = "nec_fan_light_id"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(NecFanLight),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(remote_base.REMOTE_TRANSMITTABLE_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await remote_base.register_transmittable(var, config)
