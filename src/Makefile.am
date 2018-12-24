EXTRA_DIST = config-keepassx.h  config-keepassx.h.cmake streams/QtIOCompressor


AM_CPPFLAGS = ${QT_CXXFLAGS} ${GCRYPT_CFLAGS}
AM_LDFLAGS  = ${QT_LIBS} ${GCRYPT_LIBS}

AUTOMAKE_OPTIONS=subdir-objects


keepassx_sources = \
core/AutoTypeAssociations.cpp \
core/AutoTypeAssociations.h \
core/Config.cpp \
core/Config.h \
core/Database.cpp \
core/Database.h \
core/DatabaseIcons.cpp \
core/DatabaseIcons.h \
core/Endian.cpp \
core/Endian.h \
core/Entry.cpp \
core/Entry.h \
core/EntryAttachments.cpp \
core/EntryAttachments.h \
core/EntryAttributes.cpp \
core/EntryAttributes.h \
core/EntrySearcher.cpp \
core/EntrySearcher.h \
core/Exporter.h \
core/FilePath.cpp \
core/FilePath.h \
core/Global.h \
core/Group.cpp \
core/Group.h \
core/InactivityTimer.cpp \
core/InactivityTimer.h \
core/ListDeleter.h \
core/Metadata.cpp \
core/Metadata.h \
core/PasswordGenerator.cpp \
core/PasswordGenerator.h \
core/SignalMultiplexer.cpp \
core/SignalMultiplexer.h \
core/TimeDelta.cpp \
core/TimeDelta.h \
core/TimeInfo.cpp \
core/TimeInfo.h \
core/ToDbExporter.cpp \
core/ToDbExporter.h \
core/Tools.cpp \
core/Tools.h \
core/Translator.cpp \
core/Translator.h \
core/Uuid.cpp \
core/Uuid.h \
crypto/Crypto.cpp \
crypto/Crypto.h \
crypto/CryptoHash.cpp \
crypto/CryptoHash.h \
crypto/Random.cpp \
crypto/Random.h \
crypto/SymmetricCipher.cpp \
crypto/SymmetricCipher.h \
crypto/SymmetricCipherBackend.h \
crypto/SymmetricCipherGcrypt.cpp \
crypto/SymmetricCipherGcrypt.h \
format/CsvExporter.cpp \
format/CsvExporter.h \
format/KeePass1.h \
format/KeePass1Reader.cpp \
format/KeePass1Reader.h \
format/KeePass2.h \
format/KeePass2RandomStream.cpp \
format/KeePass2RandomStream.h \
format/KeePass2Reader.cpp \
format/KeePass2Reader.h \
format/KeePass2Repair.cpp \
format/KeePass2Repair.h \
format/KeePass2Writer.cpp \
format/KeePass2Writer.h \
format/KeePass2XmlReader.cpp \
format/KeePass2XmlReader.h \
format/KeePass2XmlWriter.cpp \
format/KeePass2XmlWriter.h \
keys/CompositeKey.cpp \
keys/CompositeKey.h \
keys/CompositeKey_p.h \
keys/FileKey.cpp \
keys/FileKey.h \
keys/Key.h \
keys/PasswordKey.cpp \
keys/PasswordKey.h \
streams/HashedBlockStream.cpp \
streams/HashedBlockStream.h \
streams/LayeredStream.cpp \
streams/LayeredStream.h \
streams/StoreDataStream.cpp \
streams/StoreDataStream.h \
streams/SymmetricCipherStream.cpp \
streams/SymmetricCipherStream.h \
streams/qtiocompressor.cpp \
streams/qtiocompressor.h


moc_sources = \
streams/HashedBlockStream_moc.cpp \
streams/LayeredStream_moc.cpp \
streams/StoreDataStream_moc.cpp \
streams/SymmetricCipherStream_moc.cpp \
streams/qtiocompressor_moc.cpp \
keys/CompositeKey_p_moc.cpp \
core/Metadata_moc.cpp \
core/Entry_moc.cpp \
core/AutoTypeAssociations_moc.cpp \
core/Config_moc.cpp \
core/EntryAttachments_moc.cpp \
core/Database_moc.cpp \
core/InactivityTimer_moc.cpp \
core/Group_moc.cpp \
core/EntryAttributes_moc.cpp

BUILT_SOURCES = ${moc_sources}

clean-local :
	-rm -f ${moc_sources}


noinst_LIBRARIES = libkeepassxio.a
libkeepassxio_a_SOURCES = ${keepassx_sources} ${moc_sources}



bin_PROGRAMS = kdbxmerge
kdbxmerge_SOURCES = kdbxmerge.h kdbxmerge.cc do_merge.cc askpass_tty.h askpass_tty.c
kdbxmerge_LDADD = libkeepassxio.a

noinst_PROGRAMS = kdbxextract
kdbxextract_SOURCES = kdbx-extract.cpp
kdbxextract_LDADD = libkeepassxio.a



streams/HashedBlockStream_moc.cpp : streams/HashedBlockStream.h
	moc $< -o $@
streams/LayeredStream_moc.cpp : streams/LayeredStream.h
	moc $< -o $@
streams/StoreDataStream_moc.cpp : streams/StoreDataStream.h
	moc $< -o $@
streams/SymmetricCipherStream_moc.cpp : streams/SymmetricCipherStream.h
	moc $< -o $@
streams/qtiocompressor_moc.cpp : streams/qtiocompressor.h
	moc $< -o $@
keys/CompositeKey_p_moc.cpp : keys/CompositeKey_p.h
	moc $< -o $@
core/Metadata_moc.cpp : core/Metadata.h
	moc $< -o $@
core/Entry_moc.cpp : core/Entry.h
	moc $< -o $@
core/AutoTypeAssociations_moc.cpp : core/AutoTypeAssociations.h
	moc $< -o $@
core/Config_moc.cpp : core/Config.h
	moc $< -o $@
core/EntryAttachments_moc.cpp : core/EntryAttachments.h
	moc $< -o $@
core/Database_moc.cpp : core/Database.h
	moc $< -o $@
core/InactivityTimer_moc.cpp : core/InactivityTimer.h
	moc $< -o $@
core/Group_moc.cpp : core/Group.h
	moc $< -o $@
core/EntryAttributes_moc.cpp : core/EntryAttributes.h
	moc $< -o $@

