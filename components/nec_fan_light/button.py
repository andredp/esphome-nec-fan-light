import esphome.codegen as cg
from esphome.components import button
import esphome.config_validation as cv
from esphome.const import CONF_ID

from . import CONF_NEC_FAN_LIGHT_ID, NecFanLight, nec_fan_light_ns

DEPENDENCIES = ["nec_fan_light"]

NecButton = nec_fan_light_ns.class_("NecButton", button.Button, cg.Component)

CONF_BRIGHTNESS_UP = "brightness_up"
CONF_BRIGHTNESS_DOWN = "brightness_down"
CONF_CCT_WARM = "cct_warm"
CONF_CCT_COLD = "cct_cold"
CONF_RGB_MODE = "rgb_mode"
CONF_RGB_SPEED_UP = "rgb_speed_up"
CONF_RGB_SPEED_DOWN = "rgb_speed_down"
CONF_TIMER = "timer"
CONF_OFF_60S = "off_60s"

BUTTON_TYPES = {
    CONF_BRIGHTNESS_UP: 0xED12,
    CONF_BRIGHTNESS_DOWN: 0xFA05,
    CONF_CCT_WARM: 0xE11E,
    CONF_CCT_COLD: 0xE51A,
    CONF_RGB_MODE: 0xF50A,
    CONF_RGB_SPEED_UP: 0xF40B,
    CONF_RGB_SPEED_DOWN: 0xEF10,
    CONF_TIMER: 0xFC03,
    CONF_OFF_60S: 0xF30C,
}

BUTTON_SCHEMA = button.button_schema(NecButton).extend(cv.COMPONENT_SCHEMA)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_NEC_FAN_LIGHT_ID): cv.use_id(NecFanLight),
        cv.Optional(CONF_BRIGHTNESS_UP): BUTTON_SCHEMA,
        cv.Optional(CONF_BRIGHTNESS_DOWN): BUTTON_SCHEMA,
        cv.Optional(CONF_CCT_WARM): BUTTON_SCHEMA,
        cv.Optional(CONF_CCT_COLD): BUTTON_SCHEMA,
        cv.Optional(CONF_RGB_MODE): BUTTON_SCHEMA,
        cv.Optional(CONF_RGB_SPEED_UP): BUTTON_SCHEMA,
        cv.Optional(CONF_RGB_SPEED_DOWN): BUTTON_SCHEMA,
        cv.Optional(CONF_TIMER): BUTTON_SCHEMA,
        cv.Optional(CONF_OFF_60S): BUTTON_SCHEMA,
    }
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_NEC_FAN_LIGHT_ID])
    for key, command in BUTTON_TYPES.items():
        if key not in config:
            continue
        conf = config[key]
        var = cg.new_Pvariable(conf[CONF_ID], parent, command)
        await cg.register_component(var, conf)
        await button.register_button(var, conf)
