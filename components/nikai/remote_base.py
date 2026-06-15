import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components.remote_base import (
    declare_protocol,
    register_trigger,
    register_action,
    register_dumper,
)

DEPENDENCIES = ["remote_base"]

nikai_ns = cg.esphome_ns.namespace("remote_base")

NikaiData    = nikai_ns.struct("NikaiData")
NikaiTrigger = nikai_ns.class_("NikaiTrigger")
NikaiAction  = nikai_ns.class_("NikaiAction")
NikaiDumper  = nikai_ns.class_("NikaiDumper")

NIKAI_SCHEMA = cv.Schema({
    cv.Required("data"): cv.hex_uint32_t,
})

declare_protocol("Nikai")
register_trigger("nikai", NikaiTrigger, NikaiData)
register_action("nikai", NikaiAction, NIKAI_SCHEMA)
register_dumper("nikai", NikaiDumper)
