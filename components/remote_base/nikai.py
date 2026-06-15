import esphome.codegen as cg
import esphome.config_validation as cv

from esphome.components import remote_base


nikai_ns = cg.esphome_ns.namespace("remote_base")

NikaiData = nikai_ns.struct("NikaiData")


remote_base.declare_protocol("Nikai")


remote_base.register_dumper(
    "nikai",
    NikaiData
)

remote_base.register_trigger(
    "nikai",
    NikaiData
)

remote_base.register_action(
    "nikai",
    NikaiData,
    cv.Schema({
        cv.Required("data"): cv.hex_uint32_t
    })
)
