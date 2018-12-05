AM_CPPFLAGS = ${QT_CXXFLAGS}
AM_LDFLAGS  = ${QT_LIBS}


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
crypto/SymmetricCipherSalsa20.cpp \
crypto/SymmetricCipherSalsa20.h \
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
keys/PasswordKey.h 





bin_PROGRAMS = kdbxmerge kdbxextract
kdbxmerge_SOURCES = kdbxmerge.h kdbxmerge.cc tools_passwd.h tools_passwd.c

kdbxextract_SOURCES = kdbx-extract.cpp ${keepassx_sources}
