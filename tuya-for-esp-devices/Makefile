include $(TOPDIR)/rules.mk

PKG_NAME:=tuya-for-esp-devices
PKG_VERSION:=1.0
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/tuya-for-esp-devices
	DEPENDS:=+libTuya +libubus +libubox +libblobmsg-json +libserialport +esp-devices
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=Tuya for controlling ESP micro controllers
endef

define Package/tuya-for-esp-devices/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tuya_for_esp_devices $(1)/usr/bin/tuya_for_esp_devices
	$(INSTALL_BIN) ./files/tuya_for_esp_devices.init $(1)/etc/init.d/tuya_for_esp_devices
	$(INSTALL_BIN) ./files/tuya_for_esp_devices_script $(1)/etc/init.d/tuya_for_esp_devices_script
	$(INSTALL_CONF) ./files/tuya_for_esp_devices.config $(1)/etc/config/tuya_for_esp_devices
endef

$(eval $(call BuildPackage,tuya-for-esp-devices))