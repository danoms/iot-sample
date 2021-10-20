# debug with st-util
file out/elf

target extended-remote localhost:4242

set $periph_base = 0x40000000
set $ahb1periph_base = $periph_base + 0x20000
set $apb2periph_base = $periph_base + 0x10000

set $gpioc = $ahb1periph_base + 0x800
set $gpioi = $ahb1periph_base + 0x2000

set $usart6 = $apb2periph_base + 0x1400

define pgpiox
  p/x *(GPIO_TypeDef *)$arg0
end

define pgpiot
  p/t *(GPIO_TypeDef *)$arg0
end

define pusartx
  p/x *(USART_TypeDef *)$usart6
end

define pusartt
  p/t *(USART_TypeDef *)$usart6
end

b main.c:18
c

pgpiox $gpioc
pgpiot $gpioc
pusartt
