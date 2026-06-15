import esphome.codegen as cg

from esphome.components import remote_receiver


CODEOWNERS = ["@local"]

DEPENDENCIES = ["remote_receiver"]


nikai_ns = cg.esphome_ns.namespace("nikai")


NikaiDecoder = nikai_ns.class_(
    "NikaiDecoder",
    cg.Component
)
