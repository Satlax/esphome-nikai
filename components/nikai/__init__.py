
import esphome.codegen as cg

from esphome.components import remote_base

CODEOWNERS = ["@Satlax"]
DEPENDENCIES = ["remote_base"]
AUTO_LOAD = ["remote_base"]


nikai_ns = cg.esphome_ns.namespace("remote_base")


NikaiData = nikai_ns.struct("NikaiData")


remote_base.declare_protocol("Nikai")
