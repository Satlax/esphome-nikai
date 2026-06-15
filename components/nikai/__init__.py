import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import remote_base

DEPENDENCIES = ["remote_base"]
AUTO_LOAD = ["remote_base"]

nikai_ns = cg.esphome_ns.namespace("remote_base")

NikaiData    = nikai_ns.struct("NikaiData")
NikaiTrigger = nikai_ns.class_("NikaiTrigger")
NikaiAction  = nikai_ns.class_("NikaiAction")
NikaiDumper  = nikai_ns.class_("NikaiDumper")

NIKAI_SCHEMA = cv.Schema({
    cv.Required("data"): cv.hex_uint32_t,
})

remote_base.declare_protocol("Nikai")
remote_base.register_trigger("nikai", NikaiTrigger, NikaiData)
remote_base.register_action("nikai", NikaiAction, NIKAI_SCHEMA)
remote_base.register_dumper("nikai", NikaiDumper)
