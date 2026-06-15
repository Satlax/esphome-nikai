import esphome.codegen as cg
import esphome.config_validation as cv

from esphome.components import remote_receiver
from esphome.const import CONF_ID

DEPENDENCIES = ["remote_receiver"]

nikai_ns = cg.esphome_ns.namespace("nikai")

NikaiReceiver = nikai_ns.class_(
    "NikaiReceiver",
    cg.Component
)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(NikaiReceiver),
})
