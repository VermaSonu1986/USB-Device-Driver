#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x634251a8, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xb9177034, __VMLINUX_SYMBOL_STR(usb_deregister) },
	{ 0x3e30d1a3, __VMLINUX_SYMBOL_STR(usb_register_driver) },
	{ 0x4f6b400b, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0xf73fc157, __VMLINUX_SYMBOL_STR(usb_register_dev) },
	{ 0x4f8b5ddb, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0x3e90b75e, __VMLINUX_SYMBOL_STR(usb_bulk_msg) },
	{ 0x93123a4f, __VMLINUX_SYMBOL_STR(usb_deregister_dev) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("usb:v0781p5567d*dc*dsc*dp*ic*isc*ip*in*");

MODULE_INFO(srcversion, "00AFA1CAEF37D3E8D88EB02");
