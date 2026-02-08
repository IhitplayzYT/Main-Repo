APP := Goscrapper
INSTALL_PATH := /usr/bin/$(APP)
USER_HOME_CMD = getent passwd $$SUDO_USER | cut -d: -f6
.PHONY:$(APP) install uninstall 

webdownloader

$(APP): 
	go build .

install:
        @if [ "$$EUID" -ne 0 ]; then \
                echo "Run as ROOT: sudo make install"; \
                exit 1; \
        fi; \
        REAL_USER=$$SUDO_USER; \
        USER_HOME=$$($(USER_HOME_CMD)); \
        echo "Installing for user: $$REAL_USER"; \
        echo "Home: $$USER_HOME"; \
        \
        echo "Installing executable to $(INSTALL_PATH)"; \
        cp $(APP) $(INSTALL_PATH); \
        chmod +x $(INSTALL_PATH); \
        \
        echo "Copying templates to $$USER_HOME/buildsystemplates"; \
        mkdir -p $$USER_HOME/buildsystemplates; \
        cp -r buildsystemplates/* $$USER_HOME/buildsystemplates/; \
        \
        chown -R $$REAL_USER:$$REAL_USER $$USER_HOME/buildsystemplates; \
        \
        echo "Install complete. Run: $(APP)"

uninstall:
        @if [ "$$EUID" -ne 0 ]; then \
                echo "Run with sudo: sudo make uninstall"; \
                exit 1; \
        fi; \
        REAL_USER=$$SUDO_USER; \
        USER_HOME=$$($(USER_HOME_CMD)); \
        echo "Removing executable"; \
        rm -f $(INSTALL_PATH); \
        echo "Removing $$USER_HOME/buildsystemplates"; \
        rm -rf $$USER_HOME/buildsystemplates; \
        echo "Uninstalled."