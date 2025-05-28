# Makefile rules to support converting binary images to easily-loaded C code


# Capture the path to the directory holding this makefile so we can reference
# the tool(s) in this directory easily
SUPPORT_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

# Hard code some paths for a moment; can think about search later
build/cortex-m4/blink.tab: $(TOCK_USERLAND_BASE_DIR)/examples/blink/build/blink.tab
	cp $< $@
build/cortex-m4/adc.tab: $(TOCK_USERLAND_BASE_DIR)/examples/tests/adc/adc/build/adc.tab
	cp $< $@

%.embed: %.tab
	$(SUPPORT_DIR)/tab_to_binary_array.py $< $@
