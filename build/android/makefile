# Android Makefile for CYGWIN by Rikku2000

# Note: To create a apk file enter: "make APP=tailtale".
# ANT Password: Android

# Config Makefile for Build
ASSETSDIR = ./assets
UNSIGNED_APK = ./bin/TailTale-release-unsigned.apk
RELEASE_APK = ./bin/jp_katane_bigviosoft.tailtale.signed.apk

# Build the Project and make Release
make:
	$(ANDROID_NDK_ROOT)/ndk-build
#	test -d $(ASSETSDIR) || mkdir $(ASSETSDIR)
#	cp -r ../../data/* $(ASSETSDIR)
	$(ANT_ROOT)/ant release
	$(JAVA_HOME)bin/jarsigner -verbose -sigalg SHA1withRSA -digestalg SHA1 \
	-keystore tailtale.keystore $(UNSIGNED_APK) tailtalealias
	zipalign -f 4 $(UNSIGNED_APK) $(RELEASE_APK)
	rm -f $(UNSIGNED_APK)

# Clean the Project
clean:
	$(ANDROID_NDK_ROOT)/ndk-build clean
#	rm -rf $(ASSETSDIR)
