#**************************************************************
#  
#  Licensed to the Apache Software Foundation (ASF) under one
#  or more contributor license agreements.  See the NOTICE file
#  distributed with this work for additional information
#  regarding copyright ownership.  The ASF licenses this file
#  to you under the Apache License, Version 2.0 (the
#  "License"); you may not use this file except in compliance
#  with the License.  You may obtain a copy of the License at
#  
#    http://www.apache.org/licenses/LICENSE-2.0
#  
#  Unless required by applicable law or agreed to in writing,
#  software distributed under the License is distributed on an
#  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
#  KIND, either express or implied.  See the License for the
#  specific language governing permissions and limitations
#  under the License.
#  
#**************************************************************



gb_Deliver_GNUCOPY := $(GNUCOPY)

# if ($true) then old files will get removed from the target location before
# they are copied there. In multi-user environments, this is needed you need to
# be the owner of the target file to be able to modify timestamps
gb_Deliver_CLEARONDELIVER := $(true)

define gb_Deliver_init
gb_Deliver_DELIVERABLES :=
gb_Deliver_DELIVERABLES_INDEX := 

endef

define gb_Deliver_do_add
$$(if $(3),,$$(error - missing third parameter for deliverable $(1)))
gb_Deliver_DELIVERABLES_$(notdir $(3)) += $$(patsubst $(REPODIR)/%,%,$(2)):$$(patsubst $(REPODIR)/%,%,$(1))
gb_Deliver_DELIVERABLES_INDEX := $(sort $(gb_Deliver_DELIVERABLES_INDEX) $(notdir $(3)))
$(if $(gb_HIRESTIME),,.LOW_RESOLUTION_TIME : $(1))

endef

define gb_Deliver_add_deliverable
ifeq ($(MAKECMDGOALS),showdeliverables)
$(call gb_Deliver_do_add,$(OUTDIR)/$(1),$(2),$(3))
else
ifneq ($(CWS_WORK_STAMP),)
else
$(call gb_Deliver_do_add,$(OUTDIR)/$(1),$(2),$(3))
endif
endif

endef

ifeq ($(strip $(gb_Deliver_GNUCOPY)),)
define gb_Deliver__deliver
mkdir -p $(dir $(2)) && $(if $(gb_Deliver_CLEARONDELIVER),rm -f $(2) &&) cp -R -P -f $(1) $(2) && touch -r $(1) $(2)
endef
else
define gb_Deliver__deliver
mkdir -p $(dir $(2)) && $(gb_Deliver_GNUCOPY) $(if $(gb_Deliver_CLEARONDELIVER),--remove-destination) -R -P --force --preserve=timestamps $(1) $(2)
endef
endif

define gb_Deliver_deliver
$(if $(1),$(call gb_Deliver__deliver,$(1),$(2)),\
 $(error gb_Deliver_deliver:\
  file does not exist in solver, and cannot be delivered: $(2)))
endef


# We are currently only creating a deliver.log, if only one module gets build.
# As it is possible to add gbuild modules into other (which is done for example for
# the toplevel ooo module already) it does not make sense to create a deliver.log once
# fully migrated. The whole process should be rethought then.
# We need the trailing whitespace so that the newline of echo does not become part of the last record.
define gb_Deliver_setdeliverlogcommand
ifeq ($$(words $(gb_Module_ALLMODULES)),1)
$$(eval $$(call gb_Output_announce,$$(strip $$(gb_Module_ALLMODULES)),$$(true),LOG,1))
deliverlog : COMMAND := \
 mkdir -p $$(OUTDIR)/inc/$$(strip $$(gb_Module_ALLMODULES)) \
 && RESPONSEFILE=$$(call var2file,$(shell $(gb_MKTEMP)),100,$$(sort $$(foreach list,$$(gb_Deliver_DELIVERABLES_INDEX),$$(gb_Deliver_DELIVERABLES_$$(list))))) \
 && $(gb_AWK) -f $$(GBUILDDIR)/processdelivered.awk < $$$${RESPONSEFILE} \
        > $$(OUTDIR)/inc/$$(strip $(gb_Module_ALLMODULES))/gb_deliver.log \
 && rm -f $$$${RESPONSEFILE}
else
$$(eval $$(call gb_Output_announce,more than one module - creating no deliver.log,$$(true),LOG,1))
deliverlog : COMMAND := true
endif
endef

# FIXME: this does not really work for real multi repository builds, but the
# deliver.log format is broken in that case anyway
.PHONY : deliverlog showdeliverables
deliverlog:
	$(eval $(call gb_Deliver_setdeliverlogcommand))
	$(call gb_Helper_abbreviate_dirs, $(COMMAND))

# all : deliverlog

define gb_Deliver_print_deliverable
$(info $(1) $(patsubst $(OUTDIR)/%,%,$(2)))
endef

showdeliverables :
	$(eval MAKEFLAGS := s)
	$(foreach deliverable,$(sort $(foreach list,$(gb_Deliver_DELIVERABLES_INDEX),$(gb_Deliver_DELIVERABLES_$(list)))),\
			$(call gb_Deliver_print_deliverable,$(REPODIR)/$(firstword $(subst :, ,$(deliverable))),$(REPODIR)/$(lastword $(subst :, ,$(deliverable)))))
	true
# vim: set noet sw=4 ts=4:
