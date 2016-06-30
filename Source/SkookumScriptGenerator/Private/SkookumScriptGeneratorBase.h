//=======================================================================================
// SkookumScript Unreal Engine Binding Generator Helper
// Copyright (c) 2015 Agog Labs Inc. All rights reserved.
//
// Author: Markus Breyer
//=======================================================================================

#pragma once

//---------------------------------------------------------------------------------------
// This class provides functionality for processing UE4 runtime type information
// and for generating Sk script files

class FSkookumScriptGeneratorBase
  {
  public:

    //---------------------------------------------------------------------------------------
    // Types

    enum eSkTypeID
      {
      SkTypeID_None = 0,
      SkTypeID_Integer,
      SkTypeID_Real,
      SkTypeID_Boolean,
      SkTypeID_String,
      SkTypeID_Vector2,
      SkTypeID_Vector3,
      SkTypeID_Vector4,
      SkTypeID_Rotation,
      SkTypeID_RotationAngles,
      SkTypeID_Transform,
      SkTypeID_Color,
      SkTypeID_Name,
      SkTypeID_Enum,
      SkTypeID_UStruct,
      SkTypeID_UClass,
      SkTypeID_UObject,
      SkTypeID_UObjectWeakPtr,
      SkTypeID_List,

      SkTypeID__Count
      };

    typedef bool (*tSourceControlCheckoutFunc)(const FString & file_path);

    struct FSuperClassEntry
      {
      FString   m_name;
      UStruct * m_class_or_struct_p;

      FSuperClassEntry(const FString & name, UStruct * class_or_struct_p) : m_name(name), m_class_or_struct_p(class_or_struct_p) {}
      };

    //---------------------------------------------------------------------------------------
    // Methods

    bool                  compute_scripts_path_depth(FString project_ini_file_path, const FString & overlay_name);
    bool                  save_text_file_if_changed(const FString & file_path, const FString & new_file_contents); // Helper to change a file only if needed
    void                  flush_saved_text_files(tSourceControlCheckoutFunc checkout_f = nullptr); // Puts generated files into place after all code generation is done

    static bool           is_property_type_supported(UProperty * property_p);
    static bool           is_struct_type_supported(UStruct * struct_p);
    static bool           is_pod(UStruct * struct_p);
    static bool           does_class_have_static_class(UClass * class_p);
    static UEnum *        get_enum(UField * field_p); // Returns the Enum if it is an enum, nullptr otherwise

    static FString        skookify_class_name(const FString & name);
    static FString        skookify_var_name(const FString & name, bool append_question_mark, bool is_member = false);
    static FString        skookify_method_name(const FString & name, UProperty * return_property_p = nullptr);
    static FString        get_skookum_class_name(UStruct * class_or_struct_p);
    FString               get_skookum_class_path(UStruct * class_or_struct_p, FString * out_class_name_p = nullptr);
    FString               get_skookum_method_path(UStruct * class_or_struct_p, const FString & script_function_name, bool is_static);
    static eSkTypeID      get_skookum_struct_type(UStruct * struct_p);
    static eSkTypeID      get_skookum_property_type(UProperty * property_p);
    static FString        get_skookum_property_type_name_existing(UProperty * property_p);
    static uint32         get_skookum_symbol_id(const FString & string);
    static FString        get_comment_block(UField * field_p);

    void                  generate_class_meta_file(UStruct * class_or_struct_p, const FString & class_path, const FString & skookum_class_name);

    //---------------------------------------------------------------------------------------
    // Data

    static const FFileHelper::EEncodingOptions::Type  ms_script_file_encoding;

    static const FString  ms_sk_type_id_names[SkTypeID__Count]; // Names belonging to the ids above
    static const FString  ms_reserved_keywords[]; // = Forbidden variable names
    static const FName    ms_meta_data_key_function_category;
    static const FName    ms_meta_data_key_blueprint_type;

    FString               m_overlay_path;       // Folder where to place generated script files
    int32                 m_overlay_path_depth; // Amount of super classes until we start flattening the script file hierarchy due to the evil reign of Windows MAX_PATH. 1 = everything is right under 'Object', 0 is not allowed

    TArray<UStruct *>     m_used_classes;       // All classes used as types (by parameters, properties etc.)
    TArray<FString>       m_temp_file_paths;    // Keep track of temp files generated by save_files_if_changed()
  };
