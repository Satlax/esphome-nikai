import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.components.remote_base import declare_protocol, register_trigger, register_dumper

CODEOWNERS = ["@Satlax"]
DEPENDENCIES = ["remote_base"]
MULTI_CONF = True

# Этот метод объявляет все необходимые классы для протокола (данные, сенсор, триггер, dumper)
NikaiData, NikaiBinarySensor, NikaiTrigger, NikaiAction, NikaiDumper = declare_protocol("Nikai")

# Регистрируем триггер, который будет использоваться в YAML как 'on_nikai'
@register_trigger("nikai", NikaiTrigger, NikaiData)
def nikai_trigger(var, config):
    pass

# Регистрируем дампер, который позволит выводить декодированные коды в лог
@register_dumper("nikai", NikaiDumper)
def nikai_dumper(var, config):
    pass
