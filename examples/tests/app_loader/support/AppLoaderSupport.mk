
# Hard code some paths for a moment; can think about search later
build/blink.tab: $(TOCK_USERLAND_BASE_DIR)/examples/blink/build/blink.tab | build/
	cp $< $@
build/adc.tab: $(TOCK_USERLAND_BASE_DIR)/examples/tests/adc/adc/build/adc.tab | build/
	cp $< $@

build/cortex-m4/%.embed: build/%.tab | build/cortex-m4/
	tar -xf $< cortex-m4.tbf
	mv cortex-m4.tbf $@
