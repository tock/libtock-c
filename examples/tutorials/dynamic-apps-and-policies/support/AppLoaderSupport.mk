# Rules that need to be generated exactly once per arch, i.e.
# foreach (arch1, ...) {
#   $(1)=archX

HELPER_DIR := $(TOCK_USERLAND_BASE_DIR)/examples/tutorials/dynamic-apps-and-policies/support

# Change this to link from the dyn app loader example when that is merged.

define EMBED_RULES_PER_ARCH

####### LEGACY COMPILER SUPPORT
####### TODO: Delete when #embed is safer to assume as ubiquitous
override CFLAGS_$(1) += '-Ibuild/$(1)/'
####### END LEGACY BLOCK

####### EMERGING COMPILER SUPPORT
####### TODO: Uncomment below when the above is deleted
#######       (older compilers fail on the unknown flag)
####### override CFLAGS_$(1) += '--embed-dir=build/$(1)/'
endef
$(foreach platform, $(TOCK_ARCHS), $(eval $(call EMBED_RULES_PER_ARCH,$(platform))))


# App-specific and arch-specific rules, i.e.
# foreach (app1|app1_path, app2|app2_path) {
#   foreach (arch1, arch2) {
#     $(1)=appX
#     $(2)=appX_path
#     $$(1)=archX
define EMBED_RULES_PER_APP

## Rule to build the app to embed if-needed
.PHONY: $(2)
$(2):
	@$(MAKE) -C $(2) -q build/$(1).tab || $(MAKE) -C $(2)

define EMBED_RULES_PER_ARCH_FOR_$(1)
$$(call reject_trailing_slash, $(2))
$$(OBJS_$$(1)): $$(BUILDDIR)/$$(1)/$(1).embed

$$(BUILDDIR)/$$(1)/$(1).embed: $(2)/build/$$(1)/$$(1).tbf
	cp $$$$< $$$$@

## Use order-only rule for PHONY target to get run-once semantics
$(2)/build/$$(1)/$$(1).tbf: | $(2)

####### LEGACY COMPILER SUPPORT
####### TODO: Delete when #embed is safer to assume as ubiquitous

$$(OBJS_$$(1)): $$(BUILDDIR)/$$(1)/$(1).xxd

# $$(BUILDDIR)/$$(1)/$(1).xxd: $$(BUILDDIR)/$$(1)/$(1).embed
# 	cd $$(BUILDDIR)/$$(1) && xxd -i $(1).embed | sed 's/unsigned char/const unsigned char/' > $(1).xxd
####### END LEGACY BLOCK
endef
#$$(info $$(foreach platform, $$(TOCK_ARCHS),$$(call EMBED_RULES_PER_ARCH_FOR_$(1),$$(platform))))
$$(foreach platform, $$(TOCK_ARCHS),$$(eval $$(call EMBED_RULES_PER_ARCH_FOR_$(1),$$(platform))))
endef
#$(info $(foreach app, $(APPS_TO_EMBED),$(call EMBED_RULES_PER_APP,$(notdir $(app)),$(app))))
$(foreach app, $(APPS_TO_EMBED),$(eval $(call EMBED_RULES_PER_APP,$(notdir $(app)),$(app))))

$(BUILDDIR)/%.xxd: $(BUILDDIR)/%.embed | $(HELPER_DIR)/trim_format.py
	@mkdir -p $(@D)
	@python3 $(HELPER_DIR)/trim_format.py \
	  "$<" "$@" \
	  "$(basename $(notdir $@))_embed"

