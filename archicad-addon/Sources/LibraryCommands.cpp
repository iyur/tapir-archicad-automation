#include "LibraryCommands.hpp"
#include "ObjectState.hpp"
#include <numeric>
#include <string>

GS::Optional<GS::UniString> GetLibrariesCommand::GetResponseSchema () const
{
    return R"({
        "type": "object",
        "properties": {
            "libraries": {
                "type": "array",
                "description": "A list of project libraries.",
                "items": {
                    "type": "object",
                    "description": "Library",
                    "properties": {
                        "name": {
                            "type": "string",
                            "description": "Library name."
                        },
                        "path": {
                            "type": "string",
                            "description": "A filesystem path to library location."
                        },
                        "type": {
                            "type": "string",
                            "description": "Library type."
                        },
                        "available": {
                            "type": "boolean",
                            "description": "Is library not missing."
                        },
                        "readOnly": {
                            "type": "boolean",
                            "description": "Is library not writable."
                        },
                        "twServerUrl": {
                            "type": "string",
                            "description": "URL address of the TeamWork server hosting the library."
                        },
                        "urlWebLibrary": {
                            "type": "string",
                            "description": "URL of the downloaded Internet library."
                        }
                    },
                    "additionalProperties": false,
                    "required": [
                        "name",
                        "type",
                        "path"
                    ]
                }
            }
        },
        "additionalProperties": false,
        "required": [
            "libraries"
        ]
    })";
}

GetLibrariesCommand::GetLibrariesCommand () :
    CommandBase (CommonSchema::NotUsed)
{
}

GS::String GetLibrariesCommand::GetName () const
{
    return "GetLibraries";
}

GS::ObjectState GetLibrariesCommand::Execute (const GS::ObjectState& /*parameters*/, GS::ProcessControl& /*processControl*/) const
{
    GS::Array<API_LibraryInfo> libs;

    GSErrCode err = ACAPI_Environment (APIEnv_GetLibrariesID, &libs);
    if (err != NoError) {
        return CreateErrorResponse (err, "Failed to retrive libraries.");
    }

    GS::ObjectState response;
    const auto& listAdder = response.AddList<GS::ObjectState> ("libraries");

    for (UInt32 i = 0; i < libs.GetSize (); i++) {

        GS::ObjectState libraryData;
        GS::UniString type;
        GS::UniString twServerUrl;
        GS::UniString urlWebLibrary;
        switch (libs[i].libraryType) {
            case API_LibraryTypeID::API_Undefined:
                type = "Undefined";
                break;
            case API_LibraryTypeID::API_LocalLibrary:
                type = "LocalLibrary";
                break;
            case API_LibraryTypeID::API_UrlLibrary:
                type = "UrlLibrary";
                urlWebLibrary = libs[i].twServerUrl;
                break;
            case API_LibraryTypeID::API_BuiltInLibrary:
                type = "BuiltInLibrary";
                break;
            case API_LibraryTypeID::API_EmbeddedLibrary:
                type = "EmbeddedLibrary";
                break;
            case API_LibraryTypeID::API_OtherObject:
                type = "OtherObject";
                break;
            case API_LibraryTypeID::API_UrlOtherObject:
                type = "UrlOtherObject";
                urlWebLibrary = libs[i].twServerUrl;
                break;
            case API_LibraryTypeID::API_ServerLibrary:
                type = "ServerLibrary";
                twServerUrl = libs[i].twServerUrl;
                break;
        }

        libraryData.Add ("name", libs[i].name);
        libraryData.Add ("path", libs[i].location.ToDisplayText());
        libraryData.Add ("type", type);
        libraryData.Add ("available", libs[i].available);
        libraryData.Add ("readOnly", libs[i].readOnly);
        libraryData.Add ("twServerUrl", twServerUrl);
        libraryData.Add ("urlWebLibrary", urlWebLibrary);
        listAdder (libraryData);
    }

    return response;
}

GS::Optional<GS::UniString> GetLibPartsCommand::GetInputParametersSchema () const
{
    return R"({
        "type": "object",
        "properties": {
            "indexes": {
                "type": "array",
                "description": "Array of library part indexes."
            }
        },
        "additionalProperties": false,
        "required": []
    })";
}

GS::Optional<GS::UniString> GetLibPartsCommand::GetResponseSchema () const
{
    return R"({
        "type": "object",
        "properties": {
            "libparts": {
                "type": "array",
                "description": "test",
                "items": {
                    "type": "object",
                    "description": "Library part object.",
                    "properties": {
                        "index": {
                            "type": "integer",
                            "description": "Current database index of the library part."
                        },
                        "type": {
                            "type": "string",
                            "description": "Library part type."
                        },
                        "docuName": {
                            "type": "string",
                            "description": "Library part name."
                        },
                        "fileName": {
                            "type": "string",
                            "description": "Library part filesystem name."
                        },
                        "isMissing": {
                            "type": "boolean",
                            "description": "Is this library part missing."
                        },
                        "isTemplate": {
                            "type": "boolean",
                            "description": "Is this library part a template."
                        },
                        "isPlaceable": {
                            "type": "boolean",
                            "description": "Is this library part can be placed."
                        },
                        "ownerID": {
                            "type": "integer",
                            "description": "Signature ID of the owner, used by external objects."
                        },
                        "version": {
                            "type": "integer",
                            "description": "Library part version."
                        },
                        "platformSign": {
                            "type": "integer",
                            "description": "Library part platform ID."
                        },
                        "location": {
                            "type": "string",
                            "description": "A filesystem path to this library part."
                        },
                        "ownUnID": {
                            "type": "string",
                            "description": "The own GUID of the library part."
                        },
                        "parentUnID": {
                            "type": "string",
                            "description": "The GUID of the parent library part subtype in the hierarchy."
                        },
                        "additionalProperties": false,
                        "required": [
                            "index",
                            "type",
                            "docuName"
                        ]
                    }
                }
            }
        },
        "additionalProperties": false,
        "required": [
            "libparts"
        ]
    })";
}

GetLibPartsCommand::GetLibPartsCommand () :
    CommandBase (CommonSchema::NotUsed)
{
}

GS::String GetLibPartsCommand::GetName () const
{
    return "GetLibParts";
}

GS::ObjectState GetLibPartsCommand::Execute (const GS::ObjectState& parameters, GS::ProcessControl& /*processControl*/) const
{
    Int32 count;
    GS::Array<Int32> indexes;
    parameters.Get ("indexes", indexes);

    // use the given index(es) or go through the all
    if (indexes.GetSize () <= 0 || indexes.IsEmpty ()) {
        GSErrCode err = ACAPI_LibPart_GetNum (&count);
        indexes.SetSize (count);
        std::iota (indexes.Begin (), indexes.End (), 1);
        if (err != NoError) {
            return CreateErrorResponse (err, "Failed to retrive any library parts.");
        }
    }

    API_LibPart libPart;
    GS::ObjectState response;
    const auto& listAdder = response.AddList<GS::ObjectState> ("libparts");

    for (const auto& i : indexes) {

        BNZeroMemory (&libPart, sizeof (API_LibPart));
        libPart.index = i;
        GSErrCode err = ACAPI_LibPart_Get (&libPart);
        
        if (err != NoError) {
            std::string failure = std::string ("Failed to retrieve library part with given index: ") + " " + std::to_string (i);
            listAdder (CreateErrorResponse (err, failure.c_str ()));
            continue;
        }
        else {
            GS::UniString docuName = GS::UniString (libPart.docu_UName).ToCStr ();
            GS::UniString fileName = GS::UniString (libPart.file_UName).ToCStr ();
            GS::UniString ownUnID = GS::UniString (libPart.ownUnID).ToCStr ();
            GS::UniString parentUnID = GS::UniString (libPart.parentUnID).ToCStr ();
            GS::UniString type;
            switch (libPart.typeID) {
                case API_LibTypeID::API_ZombieLibID:
                    type = "ZombieLib";
                    break;
                case API_LibTypeID::APILib_SpecID:
                    type = "Lib_Spec";
                    break;
                case API_LibTypeID::APILib_WindowID:
                    type = "Lib_WindowID";
                    break;
                case API_LibTypeID::APILib_DoorID:
                    type = "Lib_DoorID";
                    break;
                case API_LibTypeID::APILib_ObjectID:
                    type = "Lib_ObjectID";
                    break;
                case API_LibTypeID::APILib_LampID:
                    type = "Lib_LampID";
                    break;
                case API_LibTypeID::APILib_RoomID:
                    type = "Lib_RoomID";
                    break;
                case API_LibTypeID::APILib_PropertyID:
                    type = "Lib_PropertyID";
                    break;
                case API_LibTypeID::APILib_PlanSignID:
                    type = "Lib_PlanSignID";
                    break;
                case API_LibTypeID::APILib_LabelID:
                    type = "Lib_LabelID";
                    break;
                case API_LibTypeID::APILib_MacroID:
                    type = "Lib_MacroID";
                    break;
                case API_LibTypeID::APILib_PictID:
                    type = "Lib_PictID";
                    break;
                case API_LibTypeID::APILib_ListSchemeID:
                    type = "Lib_ListSchemeID";
                    break;
                case API_LibTypeID::APILib_SkylightID:
                    type = "Lib_SkylightID";
                    break;
            }

            GS::ObjectState libPartData;
            libPartData.Add ("index", libPart.index);
            libPartData.Add ("type", type);
            libPartData.Add ("docuName", docuName);
            libPartData.Add ("fileName", fileName);
            libPartData.Add ("isMissing", libPart.missingDef);
            libPartData.Add ("isTemplate", libPart.isTemplate);
            libPartData.Add ("isPlaceable", libPart.isPlaceable);
            libPartData.Add ("ownerID", libPart.ownerID);
            libPartData.Add ("version", libPart.version);
            libPartData.Add ("platformSign", libPart.platformSign);
            libPartData.Add ("location", libPart.location->ToDisplayText ());
            libPartData.Add ("ownUnID", ownUnID);
            libPartData.Add ("parentUnID", parentUnID);
            listAdder (libPartData);

            if (libPart.location != nullptr)
                delete libPart.location;

        }

    }

    return response;

}

ReloadLibrariesCommand::ReloadLibrariesCommand () :
    CommandBase (CommonSchema::NotUsed)
{
}

GS::String ReloadLibrariesCommand::GetName () const
{
    return "ReloadLibraries";
}

GS::ObjectState ReloadLibrariesCommand::Execute (const GS::ObjectState& /*parameters*/, GS::ProcessControl& /*processControl*/) const
{
    GSErrCode err = ACAPI_Automate (APIDo_ReloadLibrariesID);

    if (err != NoError) {
        return CreateErrorResponse (err, "Failed to reload libraries.");
    }

    return {};
}