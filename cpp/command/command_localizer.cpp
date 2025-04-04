//---------------------------------------------------------------------------
//
// SCSI2Pi, SCSI device emulator and SCSI tools for the Raspberry Pi
//
// Copyright (C) 2021-2025 Uwe Seimet
//
//---------------------------------------------------------------------------

#include "command_localizer.h"
#include <cassert>
#include <unordered_set>

using namespace s2p_util;

CommandLocalizer::CommandLocalizer()
{
    Add(LocalizationKey::ERROR_AUTHENTICATION, "en", "Authentication failed");
    Add(LocalizationKey::ERROR_AUTHENTICATION, "de", "Authentifizierung fehlgeschlagen");
    Add(LocalizationKey::ERROR_AUTHENTICATION, "sv", "Autentiseringen misslyckades");
    Add(LocalizationKey::ERROR_AUTHENTICATION, "fr", "Authentification éronnée");
    Add(LocalizationKey::ERROR_AUTHENTICATION, "es", "Fallo de autentificación");
    Add(LocalizationKey::ERROR_AUTHENTICATION, "zh", "认证失败");

    Add(LocalizationKey::ERROR_OPERATION, "en", "Unknown operation: %1");
    Add(LocalizationKey::ERROR_OPERATION, "de", "Unbekannte Operation: %1");
    Add(LocalizationKey::ERROR_OPERATION, "sv", "Okänd operation: %1");
    Add(LocalizationKey::ERROR_OPERATION, "fr", "Opération inconnue: %1");
    Add(LocalizationKey::ERROR_OPERATION, "es", "Operación desconocida: %1");
    Add(LocalizationKey::ERROR_OPERATION, "zh", "未知操作: %1");

    Add(LocalizationKey::ERROR_LOG_LEVEL, "en", "Invalid log level '%1'");
    Add(LocalizationKey::ERROR_LOG_LEVEL, "de", "Ungültiger Log-Level '%1'");
    Add(LocalizationKey::ERROR_LOG_LEVEL, "sv", "Ogiltig loggnivå '%1'");
    Add(LocalizationKey::ERROR_LOG_LEVEL, "fr", "Niveau de journalisation invalide '%1'");
    Add(LocalizationKey::ERROR_LOG_LEVEL, "es", "Nivel de registro '%1' no válido");
    Add(LocalizationKey::ERROR_LOG_LEVEL, "zh", "无效的日志级别 '%1'");

    Add(LocalizationKey::ERROR_MISSING_DEVICE_ID, "en", "Missing device ID");
    Add(LocalizationKey::ERROR_MISSING_DEVICE_ID, "de", "Fehlende Geräte-ID");
    Add(LocalizationKey::ERROR_MISSING_DEVICE_ID, "sv", "Enhetens id saknas");
    Add(LocalizationKey::ERROR_MISSING_DEVICE_ID, "fr", "ID de périphérique manquante");
    Add(LocalizationKey::ERROR_MISSING_DEVICE_ID, "es", "Falta el ID del dispositivo");
    Add(LocalizationKey::ERROR_MISSING_DEVICE_ID, "zh", "缺少设备 ID");

    Add(LocalizationKey::ERROR_MISSING_FILENAME, "en", "Missing filename");
    Add(LocalizationKey::ERROR_MISSING_FILENAME, "de", "Fehlender Dateiname");
    Add(LocalizationKey::ERROR_MISSING_FILENAME, "sv", "Filnamn saknas");
    Add(LocalizationKey::ERROR_MISSING_FILENAME, "fr", "Nom de fichier manquant");
    Add(LocalizationKey::ERROR_MISSING_FILENAME, "es", "Falta el nombre del archivo");
    Add(LocalizationKey::ERROR_MISSING_FILENAME, "zh", "缺少文件名");

    Add(LocalizationKey::ERROR_DEVICE_MISSING_FILENAME, "en", "Device %1 requires a filename");
    Add(LocalizationKey::ERROR_DEVICE_MISSING_FILENAME, "de", "Gerät %1 benötigt einen Dateinamen");
    Add(LocalizationKey::ERROR_DEVICE_MISSING_FILENAME, "sv", "Enhet %1 kräver ett filnamn");
    Add(LocalizationKey::ERROR_DEVICE_MISSING_FILENAME, "fr", "Périphérique %1 à besoin d'un nom de fichier");
    Add(LocalizationKey::ERROR_DEVICE_MISSING_FILENAME, "es", "Dispositivo %1 requiere un nombre de archivo");
    Add(LocalizationKey::ERROR_DEVICE_MISSING_FILENAME, "zh", "设备 %1 需要文件名");

    Add(LocalizationKey::ERROR_IMAGE_IN_USE, "en", "Image file '%1' is already being used by device %2");
    Add(LocalizationKey::ERROR_IMAGE_IN_USE, "de", "Image-Datei '%1' wird bereits von Gerät %2 benutzt");
    Add(LocalizationKey::ERROR_IMAGE_IN_USE, "sv", "Skivbildsfilen '%1' används redan av nhet %2");
    Add(LocalizationKey::ERROR_IMAGE_IN_USE, "fr", "Le fichier d'image '%1' est déjà utilisé par périphérique %2");
    Add(LocalizationKey::ERROR_IMAGE_IN_USE, "es",
        "El archivo de imagen '%1' ya está siendo utilizado por dispositivo %2");
    Add(LocalizationKey::ERROR_IMAGE_IN_USE, "zh", "图像文件%1已被 ID %2");

    Add(LocalizationKey::ERROR_IMAGE_FILE_INFO, "en", "Can't create image file info for '%1'");
    Add(LocalizationKey::ERROR_IMAGE_FILE_INFO, "de", "Image-Datei-Information für '%1' kann nicht erzeugt werden");
    Add(LocalizationKey::ERROR_IMAGE_FILE_INFO, "sv", "Kunde ej skapa skivbildsfilsinfo för '%1'");
    Add(LocalizationKey::ERROR_IMAGE_FILE_INFO, "fr", "Ne peux pas créer les informations du fichier image '%1'");
    Add(LocalizationKey::ERROR_IMAGE_FILE_INFO, "es", "No se puede crear información de archivo de imagen para '%1'");
    Add(LocalizationKey::ERROR_IMAGE_FILE_INFO, "zh", "无法为'%1'创建图像文件信息");

    Add(LocalizationKey::ERROR_RESERVED_ID, "en", "Device ID %1 is reserved");
    Add(LocalizationKey::ERROR_RESERVED_ID, "de", "Geräte-ID %1 ist reserviert");
    Add(LocalizationKey::ERROR_RESERVED_ID, "sv", "Enhets-id %1 är reserverat");
    Add(LocalizationKey::ERROR_RESERVED_ID, "fr", "ID de périphérique %1 réservée");
    Add(LocalizationKey::ERROR_RESERVED_ID, "es", "El ID de dispositivo %1 está reservado");
    Add(LocalizationKey::ERROR_RESERVED_ID, "zh", "设备 ID %1 已保留");

    Add(LocalizationKey::ERROR_NON_EXISTING_DEVICE, "en", "Command for non-existing ID %1");
    Add(LocalizationKey::ERROR_NON_EXISTING_DEVICE, "de", "Kommando für nicht existente ID %1");
    Add(LocalizationKey::ERROR_NON_EXISTING_DEVICE, "sv", "Kommando för id %1 som ej existerar");
    Add(LocalizationKey::ERROR_NON_EXISTING_DEVICE, "fr", "Commande pour ID %1 non-existant");
    Add(LocalizationKey::ERROR_NON_EXISTING_DEVICE, "es", "Comando para ID %1 no existente");
    Add(LocalizationKey::ERROR_NON_EXISTING_DEVICE, "zh", "不存在的 ID %1 的指令");

    Add(LocalizationKey::ERROR_NON_EXISTING_UNIT, "en", "Command for non-existing ID %1, unit %2");
    Add(LocalizationKey::ERROR_NON_EXISTING_UNIT, "de", "Kommando für nicht existente ID %1, Einheit %2");
    Add(LocalizationKey::ERROR_NON_EXISTING_UNIT, "sv", "Kommando för id %1, enhetsnummer %2 som ej existerar");
    Add(LocalizationKey::ERROR_NON_EXISTING_UNIT, "fr", "Command pour ID %1, unité %2 non-existant");
    Add(LocalizationKey::ERROR_NON_EXISTING_UNIT, "es", "Comando para ID %1 inexistente, unidad %2");
    Add(LocalizationKey::ERROR_NON_EXISTING_UNIT, "zh", "不存在的 ID %1, 单元 %2 的指令");

    Add(LocalizationKey::ERROR_UNKNOWN_DEVICE_TYPE, "en", "%1:%2: Unknown device type %3");
    Add(LocalizationKey::ERROR_UNKNOWN_DEVICE_TYPE, "de", "%1:%2: Unbekannter Gerätetyp %3");
    Add(LocalizationKey::ERROR_UNKNOWN_DEVICE_TYPE, "sv", "%1:%2: Obekant enhetstyp: %3");
    Add(LocalizationKey::ERROR_UNKNOWN_DEVICE_TYPE, "fr", "%1:%2: Type de périphérique inconnu %3");
    Add(LocalizationKey::ERROR_UNKNOWN_DEVICE_TYPE, "es", "%1:%2: Tipo de dispositivo desconocido %3");
    Add(LocalizationKey::ERROR_UNKNOWN_DEVICE_TYPE, "zh", "%1:%2: 未知设备类型 %3");

    Add(LocalizationKey::ERROR_MISSING_DEVICE_TYPE, "en",
        "%1:%2: Device type required for unknown extension of file '%3'");
    Add(LocalizationKey::ERROR_MISSING_DEVICE_TYPE, "de",
        "%1:%2: Gerätetyp erforderlich für unbekannte Extension der Datei '%3'");
    Add(LocalizationKey::ERROR_MISSING_DEVICE_TYPE, "sv",
        "%1:%2: Man måste ange enhetstyp för obekant filändelse '%3'");
    Add(LocalizationKey::ERROR_MISSING_DEVICE_TYPE, "fr",
        "%1:%2: Type de périphérique requis pour extension inconnue du fichier '%3'");
    Add(LocalizationKey::ERROR_MISSING_DEVICE_TYPE, "es",
        "%1:%2: Tipo de dispositivo requerido para la extensión desconocida del archivo '%3'");
    Add(LocalizationKey::ERROR_MISSING_DEVICE_TYPE, "zh", "%1:%2: 文件'%3'的未知扩展名所需的设备类型");

    Add(LocalizationKey::ERROR_DUPLICATE_ID, "en", "Duplicate ID %1, unit %2");
    Add(LocalizationKey::ERROR_DUPLICATE_ID, "de", "Doppelte ID %1, Einheit %2");
    Add(LocalizationKey::ERROR_DUPLICATE_ID, "sv", "Duplikat id %1, enhetsnummer %2");
    Add(LocalizationKey::ERROR_DUPLICATE_ID, "fr", "ID %1, unité %2 dupliquée");
    Add(LocalizationKey::ERROR_DUPLICATE_ID, "es", "ID duplicado %1, unidad %2");
    Add(LocalizationKey::ERROR_DUPLICATE_ID, "zh", "重复 ID %1,单元 %2");

    Add(LocalizationKey::ERROR_DETACH, "en", "Couldn't detach device");
    Add(LocalizationKey::ERROR_DETACH, "de", "Geräte konnte nicht entfernt werden");
    Add(LocalizationKey::ERROR_DETACH, "sv", "Kunde ej koppla ifrån enheten");
    Add(LocalizationKey::ERROR_DETACH, "fr", "Impossible de détacher le périphérique");
    Add(LocalizationKey::ERROR_DETACH, "es", "No se ha podido desconectar el dispositivo");
    Add(LocalizationKey::ERROR_DETACH, "zh", "无法卸载设备");

    Add(LocalizationKey::ERROR_EJECT_REQUIRED, "en", "Existing medium must first be ejected");
    Add(LocalizationKey::ERROR_EJECT_REQUIRED, "de", "Das vorhandene Medium muss erst ausgeworfen werden");
    Add(LocalizationKey::ERROR_EJECT_REQUIRED, "sv", "Nuvarande skiva måste utmatas först");
    Add(LocalizationKey::ERROR_EJECT_REQUIRED, "fr", "Media déjà existant doit d'abord être éjecté");
    Add(LocalizationKey::ERROR_EJECT_REQUIRED, "es", "El medio existente debe ser expulsado primero");
    Add(LocalizationKey::ERROR_EJECT_REQUIRED, "zh", "必须先弹出现有介质");

    Add(LocalizationKey::ERROR_DEVICE_NAME_UPDATE, "en", "Once set the device name cannot be changed anymore");
    Add(LocalizationKey::ERROR_DEVICE_NAME_UPDATE, "de",
        "Ein bereits gesetzter Gerätename kann nicht mehr geändert werden");
    Add(LocalizationKey::ERROR_DEVICE_NAME_UPDATE, "sv", "Enhetsnamn kan ej ändras efter att ha fastställts en gång");
    Add(LocalizationKey::ERROR_DEVICE_NAME_UPDATE, "fr",
        "Une fois défini, le nom de périphérique ne peut plus être changé");
    Add(LocalizationKey::ERROR_DEVICE_NAME_UPDATE, "es",
        "Una vez establecido el nombre del dispositivo ya no se puede cambiar");
    Add(LocalizationKey::ERROR_DEVICE_NAME_UPDATE, "zh", "设备名称设置后不能再更改");

    Add(LocalizationKey::ERROR_SHUTDOWN_MODE_INVALID, "en", "Invalid shutdown mode '%1'");
    Add(LocalizationKey::ERROR_SHUTDOWN_MODE_INVALID, "de", "Ungültiger Shutdown-Modus '%1'");
    Add(LocalizationKey::ERROR_SHUTDOWN_MODE_INVALID, "sv", "Ogiltigt avstängsningsläge: '%1'");
    Add(LocalizationKey::ERROR_SHUTDOWN_MODE_INVALID, "fr", "Mode d'extinction invalide '%1'");
    Add(LocalizationKey::ERROR_SHUTDOWN_MODE_INVALID, "es", "Modo de apagado inválido '%1'");
    Add(LocalizationKey::ERROR_SHUTDOWN_MODE_INVALID, "zh", "无效的关机模式 '%1'");

    Add(LocalizationKey::ERROR_SHUTDOWN_PERMISSION, "en", "Missing root permission for shutdown or reboot");
    Add(LocalizationKey::ERROR_SHUTDOWN_PERMISSION, "de", "Fehlende Root-Berechtigung für Shutdown oder Neustart");
    Add(LocalizationKey::ERROR_SHUTDOWN_PERMISSION, "sv",
        "Saknar root-rättigheter för att kunna stänga av eller starta om systemet");
    Add(LocalizationKey::ERROR_SHUTDOWN_PERMISSION, "fr", "Permissions root manquantes pour extinction ou redémarrage");
    Add(LocalizationKey::ERROR_SHUTDOWN_PERMISSION, "es", "Falta el permiso de root para el apagado o el reinicio");
    Add(LocalizationKey::ERROR_SHUTDOWN_PERMISSION, "zh", "缺少关机或重启的 root 权限");

    Add(LocalizationKey::ERROR_FILE_OPEN, "en", "Invalid or non-existing file '%1'");
    Add(LocalizationKey::ERROR_FILE_OPEN, "de", "Ungültige oder fehlende Datei '%1'");
    Add(LocalizationKey::ERROR_FILE_OPEN, "sv", "Ogiltig eller saknad fil '%1'");
    Add(LocalizationKey::ERROR_FILE_OPEN, "fr", "Fichier invalide ou non-existant '%1'");
    Add(LocalizationKey::ERROR_FILE_OPEN, "es", "Archivo inválido o inexistente '%1'");
    Add(LocalizationKey::ERROR_FILE_OPEN, "zh", "文件'%1'无效或不存在");

    Add(LocalizationKey::ERROR_SCSI_LEVEL, "en", "Invalid SCSI level: %1");
    Add(LocalizationKey::ERROR_SCSI_LEVEL, "de", "Ungültiger SCSI-Level: %1");
    Add(LocalizationKey::ERROR_SCSI_LEVEL, "sv", "Ogiltig SCSI nivå: %1");
    Add(LocalizationKey::ERROR_SCSI_LEVEL, "fr", "Niveau SCSI %1 invalide");
    Add(LocalizationKey::ERROR_SCSI_LEVEL, "es", "Niveau SCSI %1 invalido");
    Add(LocalizationKey::ERROR_SCSI_LEVEL, "zh", "SCSI级别 %1 字节");

    Add(LocalizationKey::ERROR_BLOCK_SIZE, "en", "Invalid block size: %1 bytes");
    Add(LocalizationKey::ERROR_BLOCK_SIZE, "de", "Ungültige Blockgröße: %1 Bytes");
    Add(LocalizationKey::ERROR_BLOCK_SIZE, "sv", "Ogiltig blockstorlek: %1 byte");
    Add(LocalizationKey::ERROR_BLOCK_SIZE, "fr", "Taille de bloc %1 octets invalide");
    Add(LocalizationKey::ERROR_BLOCK_SIZE, "es", "Tamaño de bloque %1 bytes invalido");
    Add(LocalizationKey::ERROR_BLOCK_SIZE, "zh", "无效的块大小 %1 字节");

    Add(LocalizationKey::ERROR_BLOCK_SIZE_NOT_CONFIGURABLE, "en", "Block size for device type %1 is not configurable");
    Add(LocalizationKey::ERROR_BLOCK_SIZE_NOT_CONFIGURABLE, "de",
        "Blockgröße für Gerätetyp %1 ist nicht konfigurierbar");
    Add(LocalizationKey::ERROR_BLOCK_SIZE_NOT_CONFIGURABLE, "sv",
        "Enhetstypen %1 kan inte använda andra blockstorlekar");
    Add(LocalizationKey::ERROR_BLOCK_SIZE_NOT_CONFIGURABLE, "fr",
        "Taille de block pour le type de périphérique %1 non configurable");
    Add(LocalizationKey::ERROR_BLOCK_SIZE_NOT_CONFIGURABLE, "es",
        "El tamaño del bloque para el tipo de dispositivo %1 no es configurable");
    Add(LocalizationKey::ERROR_BLOCK_SIZE_NOT_CONFIGURABLE, "zh", "设备类型 %1 的块大小不可配置");

    Add(LocalizationKey::ERROR_CONTROLLER, "en", "Couldn't create controller");
    Add(LocalizationKey::ERROR_CONTROLLER, "de", "Controller konnte nicht erzeugt werden");
    Add(LocalizationKey::ERROR_CONTROLLER, "sv", "Kunde ej skapa gränssnitt");
    Add(LocalizationKey::ERROR_CONTROLLER, "fr", "Impossible de créer le contrôleur");
    Add(LocalizationKey::ERROR_CONTROLLER, "es", "No se ha podido crear el controlador");
    Add(LocalizationKey::ERROR_CONTROLLER, "zh", "无法创建 控制器");

    Add(LocalizationKey::ERROR_INVALID_ID, "en", "Invalid device ID %1 (0-7)");
    Add(LocalizationKey::ERROR_INVALID_ID, "de", "Ungültige Geräte-ID %1 (0-7)");
    Add(LocalizationKey::ERROR_INVALID_ID, "sv", "Ogiltigt enhets-id %1 (0-7)");
    Add(LocalizationKey::ERROR_INVALID_ID, "fr", "ID de périphérique invalide %1 (0-7)");
    Add(LocalizationKey::ERROR_INVALID_ID, "es", "ID de dispositivo inválido %1 (0-7)");
    Add(LocalizationKey::ERROR_INVALID_ID, "zh", "无效的设备 ID %1 (0-7)");

    Add(LocalizationKey::ERROR_INVALID_LUN, "en", "Invalid LUN %1 (0-%2)");
    Add(LocalizationKey::ERROR_INVALID_LUN, "de", "Ungültige LUN %1 (0-%2)");
    Add(LocalizationKey::ERROR_INVALID_LUN, "sv", "Ogiltigt enhetsnummer %1 (0-%2)");
    Add(LocalizationKey::ERROR_INVALID_LUN, "fr", "LUN invalide %1 (0-%2)");
    Add(LocalizationKey::ERROR_INVALID_LUN, "es", "LUN invalido %1 (0-%2)");
    Add(LocalizationKey::ERROR_INVALID_LUN, "zh", "无效的 LUN %1 (0-%2)");

    Add(LocalizationKey::ERROR_MISSING_LUN0, "en", "Missing LUN 0 for device ID %1");
    Add(LocalizationKey::ERROR_MISSING_LUN0, "de", "Fehlende LUN 0 für Geräte-ID %1");
    Add(LocalizationKey::ERROR_MISSING_LUN0, "sv", "Saknar LUN 0 för enhets-ID %1");
    Add(LocalizationKey::ERROR_MISSING_LUN0, "fr", "LUN 0 manquant pour l'ID de périphérique %1");
    Add(LocalizationKey::ERROR_MISSING_LUN0, "es", "Falta LUN 0 para la ID del dispositivo %1");
    Add(LocalizationKey::ERROR_MISSING_LUN0, "zh", "设备 ID %1 缺少 LUN 0");

    Add(LocalizationKey::ERROR_LUN0, "en", "LUN 0 cannot be detached as long as there is still another LUN");
    Add(LocalizationKey::ERROR_LUN0, "de", "LUN 0 kann nicht entfernt werden, solange noch eine andere LUN existiert");
    Add(LocalizationKey::ERROR_LUN0, "sv",
        "Enhetsnummer 0 kan ej bli frånkopplat så länge som andra enhetsnummer är anslutna");
    Add(LocalizationKey::ERROR_LUN0, "fr", "LUN 0 ne peux pas être détaché tant qu'il y'a un autre LUN");
    Add(LocalizationKey::ERROR_LUN0, "es", "El LUN 0 no se puede desconectar mientras haya otro LUN");
    Add(LocalizationKey::ERROR_LUN0, "zh", "LUN 0 无法卸载，因为当前仍有另一个 LUN。");

    Add(LocalizationKey::ERROR_INITIALIZATION, "en", "Initialization of %1 failed");
    Add(LocalizationKey::ERROR_INITIALIZATION, "de", "Initialisierung von %1 fehlgeschlagen");
    Add(LocalizationKey::ERROR_INITIALIZATION, "sv", "Kunde ej initialisera %1 ");
    Add(LocalizationKey::ERROR_INITIALIZATION, "fr", "Echec de l'initialisation de %1");
    Add(LocalizationKey::ERROR_INITIALIZATION, "es", "La inicialización del %1 falló");
    Add(LocalizationKey::ERROR_INITIALIZATION, "zh", "%1 的初始化失败");

    Add(LocalizationKey::ERROR_OPERATION_DENIED_STOPPABLE, "en", "%1 operation denied, %2 isn't stoppable");
    Add(LocalizationKey::ERROR_OPERATION_DENIED_STOPPABLE, "de", "%1-Operation verweigert, %2 ist nicht stopbar");
    Add(LocalizationKey::ERROR_OPERATION_DENIED_STOPPABLE, "sv", "Operationen %1 nekades för att %2 inte kan stoppas");
    Add(LocalizationKey::ERROR_OPERATION_DENIED_STOPPABLE, "fr", "Opération %1 refusée, %2 ne peut être stoppé");
    Add(LocalizationKey::ERROR_OPERATION_DENIED_STOPPABLE, "es", "%1 operación denegada, %2 no se puede parar");
    Add(LocalizationKey::ERROR_OPERATION_DENIED_STOPPABLE, "zh", "%1 操作被拒绝，%2 不可停止");

    Add(LocalizationKey::ERROR_OPERATION_DENIED_REMOVABLE, "en", "%1 operation denied, %2 isn't removable");
    Add(LocalizationKey::ERROR_OPERATION_DENIED_REMOVABLE, "de", "%1-Operation verweigert, %2 ist nicht wechselbar");
    Add(LocalizationKey::ERROR_OPERATION_DENIED_REMOVABLE, "sv",
        "Operationen %1 nekades för att %2 inte är uttagbar(t)");
    Add(LocalizationKey::ERROR_OPERATION_DENIED_REMOVABLE, "fr", "Opération %1 refusée, %2 n'est pas détachable");
    Add(LocalizationKey::ERROR_OPERATION_DENIED_REMOVABLE, "es", "%1 operación denegada, %2 no es removible");
    Add(LocalizationKey::ERROR_OPERATION_DENIED_REMOVABLE, "zh", "%1 操作被拒绝，%2 不可移除");

    Add(LocalizationKey::ERROR_OPERATION_DENIED_PROTECTABLE, "en", "%1 operation denied, %2 isn't protectable");
    Add(LocalizationKey::ERROR_OPERATION_DENIED_PROTECTABLE, "de", "%1-Operation verweigert, %2 ist nicht schützbar");
    Add(LocalizationKey::ERROR_OPERATION_DENIED_PROTECTABLE, "sv",
        "Operationen %1 nekades för att %2 inte är skyddbar(t)");
    Add(LocalizationKey::ERROR_OPERATION_DENIED_PROTECTABLE, "fr", "Opération %1 refusée, %2 n'est pas protégeable");
    Add(LocalizationKey::ERROR_OPERATION_DENIED_PROTECTABLE, "es", "%1 operación denegada, %2 no es protegible");
    Add(LocalizationKey::ERROR_OPERATION_DENIED_PROTECTABLE, "zh", "%1 操作被拒绝,%2 不可保护");

    Add(LocalizationKey::ERROR_OPERATION_DENIED_READY, "en", "%1 operation denied, %2 isn't ready");
    Add(LocalizationKey::ERROR_OPERATION_DENIED_READY, "de", "%1-Operation verweigert, %2 ist nicht bereit");
    Add(LocalizationKey::ERROR_OPERATION_DENIED_READY, "sv", "Operationen %1 nekades för att %2 inte är redo");
    Add(LocalizationKey::ERROR_OPERATION_DENIED_READY, "fr", "Opération %1 refusée, %2 n'est pas prêt");
    Add(LocalizationKey::ERROR_OPERATION_DENIED_READY, "es", "%1 operación denegada, %2 no está listo");
    Add(LocalizationKey::ERROR_OPERATION_DENIED_READY, "zh", "%1 操作被拒绝,%2 还没有准备好");

    Add(LocalizationKey::ERROR_UNIQUE_DEVICE_TYPE, "en", "There can only be a single %1 device");
    Add(LocalizationKey::ERROR_UNIQUE_DEVICE_TYPE, "de", "Es kann nur ein einziges %1-Gerät geben");
    Add(LocalizationKey::ERROR_UNIQUE_DEVICE_TYPE, "sv", "Det kan bara finnas en enda %1-enhet");
    Add(LocalizationKey::ERROR_UNIQUE_DEVICE_TYPE, "fr", "Il ne peut y avoir qu'un seul périphérique %1");
    Add(LocalizationKey::ERROR_UNIQUE_DEVICE_TYPE, "es", "Sólo puede haber un único dispositivo %1");
    Add(LocalizationKey::ERROR_UNIQUE_DEVICE_TYPE, "zh", "只能有一个%1设备");

    Add(LocalizationKey::ERROR_PERSIST, "en", "Couldn't save '/etc/s2p.conf'");
    Add(LocalizationKey::ERROR_PERSIST, "de", "'/etc/s2p.conf' konnte nicht gespeichert werden");
    Add(LocalizationKey::ERROR_PERSIST, "sv", "Kunde inte spara '/etc/s2p.conf'");
    Add(LocalizationKey::ERROR_PERSIST, "fr", "Impossible d'enregistrer '/etc/s2p.conf'");
    Add(LocalizationKey::ERROR_PERSIST, "es", "No se pudo guardar '/etc/s2p.conf'");
    Add(LocalizationKey::ERROR_PERSIST, "zh", "无法保存'/etc/s2p.conf'");
}

void CommandLocalizer::Add(LocalizationKey key, const string &locale, string_view value)
{
    // Safeguards against empty messages, duplicate entries and unsupported locales
    assert(!locale.empty());
    assert(!value.empty());
    assert(!localized_messages[locale].contains(key));
    assert((unordered_set<string_view>( { "en", "de", "sv", "fr", "es", "zh" })).contains(locale));

    localized_messages[locale][key] = value;
}

string CommandLocalizer::Localize(LocalizationKey key, const string &locale, const string &arg1, const string &arg2,
    const string &arg3) const
{
    auto it = localized_messages.find(ToLower(locale).substr(0, 2));
    if (it == localized_messages.end()) {
        // Use English as fallback language
        it = localized_messages.find("en");
    }
    assert(it != localized_messages.end());

    const auto &m = it->second.find(key);
    if (m == it->second.end()) {
        return "Missing localization for enum value " + to_string(static_cast<int>(key));
    }

    string message = m->second;
    message = regex_replace(message, REGEX1, arg1);
    message = regex_replace(message, REGEX2, arg2);
    return regex_replace(message, REGEX3, arg3);
}
