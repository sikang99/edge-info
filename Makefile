#
# Makefile for edge-info
#
BUILD=0.0.2.0
USER=stoney
#--------------------------------------------------------------------------------
usage:
	@echo "usage: make [git]"

#--------------------------------------------------------------------------------
git g:
	@echo "make (git:g) [update|store]"
git-reset gr:
	git reset --soft HEAD~1
git-update gu:
	git add .
	git commit -a -m "$(BUILD),$(USER)"
	git push
git-store gs:
	git config credential.helper store
#--------------------------------------------------------------------------------

