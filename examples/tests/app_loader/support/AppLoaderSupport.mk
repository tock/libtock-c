# Rules that need to be generated exactly once per arch, i.e.
# foreach (arch1, ...) {
#   $(1)=archX
define EMBED_RULES_PER_ARCH
override CFLAGS_$(1) += '--embed-dir=build/$(1)/'

####### LEGACY COMPILER SUPPORT
####### TODO: Delete when #embed is safer to assume as ubiquitous
override CFLAGS_$(1) += '-Ibuild/$(1)/'
endef
$(foreach platform, $(TOCK_ARCHS), $(eval $(call EMBED_RULES_PER_ARCH,$(platform))))

# App-specific and arch-specific rules, i.e.
# foreach (app1|app1_path, app2|app2_path) {
#   foreach (arch1, arch2) {
#     $(1)=appX
#     $(2)=appX_path
#     $$(1)=archX
define EMBED_RULES_PER_APP
define EMBED_RULES_PER_ARCH_FOR_$(1)
$$(call reject_trailing_slash, $(2))
$$(OBJS_$$(1)): $$(BUILDDIR)/$$(1)/$(1).embed

$$(BUILDDIR)/$$(1)/$(1).embed: $(2)/build/$$(1)/$$(1).tbf
	cp $$$$< $$$$@

####### LEGACY COMPILER SUPPORT
####### TODO: Delete when #embed is safer to assume as ubiquitous

$$(OBJS_$$(1)): $$(BUILDDIR)/$$(1)/$(1).xxd

$$(BUILDDIR)/$$(1)/$(1).xxd: $$(BUILDDIR)/$$(1)/$(1).embed
	pushd $$(BUILDDIR)/$$(1) && xxd -i $(1).embed > $(1).xxd
endef
#$$(info $$(foreach platform, $$(TOCK_ARCHS),$$(call EMBED_RULES_PER_ARCH_FOR_$(1),$$(platform))))
$$(foreach platform, $$(TOCK_ARCHS),$$(eval $$(call EMBED_RULES_PER_ARCH_FOR_$(1),$$(platform))))
endef
#$(info $(foreach app, $(APPS_TO_EMBED),$(call EMBED_RULES_PER_APP,$(notdir $(app)),$(app))))
$(foreach app, $(APPS_TO_EMBED),$(eval $(call EMBED_RULES_PER_APP,$(notdir $(app)),$(app))))

