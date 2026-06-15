import esphome.codegen as cg
from esphome.components.remote_base import (
    declare_protocol,
    register_trigger,
    register_dumper,
)

DEPENDENCIES = ["remote_base"]
CODEOWNERS = ["@Satlax"]

# Объявляем протокол — это автоматически создаст классы NikaiData, NikaiTrigger, NikaiDumper
NikaiData, NikaiBinarySensor, NikaiTrigger, NikaiAction, NikaiDumper = declare_protocol("Nikai")

# Регистрируем триггер для YAML (on_nikai)
@register_trigger("nikai", NikaiTrigger, NikaiData)
def nikai_trigger(var, config):
    pass

# Регистрируем дампер для dump: nikai
@register_dumper("nikai", NikaiDumper)
def nikai_dumper(var, config):
    pass
