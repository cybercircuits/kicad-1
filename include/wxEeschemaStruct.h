/***********************************************************/
/*              wxEeschemaStruct.h:                        */
/***********************************************************/

#ifndef  WX_EESCHEMA_STRUCT_H
#define  WX_EESCHEMA_STRUCT_H

#include "wxstruct.h"
#include "param_config.h"
#include "class_undoredo_container.h"
#include "template_fieldnames.h"
#include "block_commande.h"
#include "class_sch_screen.h"


class LIB_EDIT_FRAME;
class LIB_VIEW_FRAME;
class DRAWSEGMENT;
class DrawPickedStruct;
class SCH_ITEM;
class SCH_NO_CONNECT;
class CMP_LIBRARY;
class LIB_COMPONENT;
class LIB_DRAW_ITEM;
class EDA_ITEM;
class SCH_BUS_ENTRY;
class SCH_GLOBALLABEL;
class SCH_TEXT;
class SCH_SHEET;
class SCH_SHEET_PATH;
class SCH_SHEET_PIN;
class SCH_COMPONENT;
class SCH_FIELD;
class LIB_PIN;
class SCH_JUNCTION;
class DIALOG_SCH_FIND;
class wxFindDialogEvent;
class wxFindReplaceData;


/* enum used in RotationMiroir() */
enum fl_rot_cmp {
    CMP_NORMAL,                     // Normal orientation, no rotation or mirror
    CMP_ROTATE_CLOCKWISE,           // Rotate -90
    CMP_ROTATE_COUNTERCLOCKWISE,    // Rotate +90
    CMP_ORIENT_0,                   // No rotation and no mirror id CMP_NORMAL
    CMP_ORIENT_90,                  // Rotate 90, no mirror
    CMP_ORIENT_180,                 // Rotate 180, no mirror
    CMP_ORIENT_270,                 // Rotate -90, no mirror
    CMP_MIRROR_X = 0x100,           // Mirror around X axis
    CMP_MIRROR_Y = 0x200            // Mirror around Y axis
};


/**
 * Schematic editor (EESchema) main window.
 */
class SCH_EDIT_FRAME : public EDA_DRAW_FRAME
{
public:
    WinEDAChoiceBox*      m_SelPartBox;
    SCH_SHEET_PATH*       m_CurrentSheet;    ///< which sheet we are presently working on.
    int m_Multiflag;
    int m_NetlistFormat;
    bool                  m_ShowAllPins;
    wxPoint               m_OldPos;
    LIB_EDIT_FRAME*       m_LibeditFrame;
    LIB_VIEW_FRAME*       m_ViewlibFrame;
    wxString              m_UserLibraryPath;
    wxArrayString         m_ComponentLibFiles;

protected:
    TEMPLATES             m_TemplateFieldNames;

private:
    wxString              m_DefaultSchematicFileName;
    SCH_FIELD*            m_CurrentField;
    int m_TextFieldSize;
    PARAM_CFG_ARRAY       m_projectFileParams;
    PARAM_CFG_ARRAY       m_configSettings;
    wxPageSetupDialogData m_pageSetupData;
    wxFindReplaceData*    m_findReplaceData;
    wxPoint               m_previewPosition;
    wxSize                m_previewSize;
    wxPoint               m_printDialogPosition;
    wxSize                m_printDialogSize;
    bool                  m_printMonochrome;     ///< Print monochrome instead of grey scale.
    bool                  m_printSheetReference;
    DIALOG_SCH_FIND*      m_dlgFindReplace;
    wxPoint               m_findDialogPosition;
    wxSize                m_findDialogSize;
    wxArrayString         m_findStringHistoryList;
    wxArrayString         m_replaceStringHistoryList;
    BLOCK_SELECTOR        m_blockItems;         ///< List of selected items.
    SCH_ITEM*             m_itemToRepeat;       ///< Last item to insert by the repeat command.
    int                   m_repeatLabelDelta;   ///< Repeat label number increment step.

public:
    SCH_EDIT_FRAME( wxWindow* father,
                    const wxString& title,
                    const wxPoint& pos, const wxSize& size,
                    long style = KICAD_DEFAULT_DRAWFRAME_STYLE );

    ~SCH_EDIT_FRAME();

    void             OnCloseWindow( wxCloseEvent& Event );
    void             Process_Special_Functions( wxCommandEvent& event );
    void             OnColorConfig( wxCommandEvent& aEvent );
    void             Process_Config( wxCommandEvent& event );

    void             GeneralControle( wxDC* aDC, const wxPoint& aPosition );

    PARAM_CFG_ARRAY& GetProjectFileParameters( void );
    void             SaveProjectFile( wxWindow* displayframe, bool askoverwrite = true );
    bool             LoadProjectFile( const wxString& CfgFileName, bool ForceRereadConfig );

    /**
     * Function GetDefaultFieldName
     * returns a default symbol field name for field \a aFieldNdx for all components.
     * These fieldnames are not modifiable, but template fieldnames are.
     * @param aFieldNdx The field number index
     */
    static wxString  GetDefaultFieldName( int aFieldNdx );


    /**
     * Function AddTemplateFieldName
     * inserts or appends a wanted symbol field name into the fieldnames
     * template.  Should be used for any symbol property editor.  If the name
     * already exists, it overwrites the same name.
     *
     * @param aFieldName is a full description of the wanted field, and it must not match
     *          any of the default fieldnames.
     * @return int - the index within the config container at which aFieldName was
     *          added, or -1 if the name is illegal because it matches a default fieldname.
     */
    int AddTemplateFieldName( const TEMPLATE_FIELDNAME& aFieldName )
    {
        return m_TemplateFieldNames.AddTemplateFieldName( aFieldName );
    }


    /**
     * Function GetTemplateFieldName
     * returns a template fieldnames list for read only access.
     */
    const TEMPLATE_FIELDNAMES& GetTemplateFieldNames()
    {
        return m_TemplateFieldNames.GetTemplateFieldNames();
    }


    /**
     * Function DeleteAllTemplateFieldNames
     * removes all template fieldnames.
     */
    void DeleteAllTemplateFieldNames()
    {
        m_TemplateFieldNames.DeleteAllTemplateFieldNames();
    }


    PARAM_CFG_ARRAY& GetConfigurationSettings( void );
    void             LoadSettings();
    void             SaveSettings();

    void             RedrawActiveWindow( wxDC* DC, bool EraseBg );

    void             CreateScreens();
    void             ReCreateHToolbar();
    void             ReCreateVToolbar();
    void             ReCreateOptToolbar();
    void             ReCreateMenuBar();
    void             OnHotKey( wxDC* aDC, int aHotKey, const wxPoint& aPosition,
                               EDA_ITEM* aItem = NULL );

    SCH_FIELD* GetCurrentField() { return m_CurrentField; }

    void             SetCurrentField( SCH_FIELD* aCurrentField )
    {
        m_CurrentField = aCurrentField;
    }


    /**
     * Function OnModify
     * Must be called after a schematic change
     * in order to set the "modify" flag of the current screen
     * and update the date in frame reference
     */
    void             OnModify();

    SCH_SHEET_PATH*  GetSheet();

    SCH_SCREEN*      GetScreen() const;

    virtual wxString GetScreenDesc();

    void             InstallConfigFrame( wxCommandEvent& event );

    void             OnLeftClick( wxDC* aDC, const wxPoint& aPosition );
    void             OnLeftDClick( wxDC* aDC, const wxPoint& aPosition );
    bool             OnRightClick( const wxPoint& aPosition, wxMenu* PopMenu );
    void             OnSelectOptionToolbar( wxCommandEvent& event );
    int              BestZoom();

    SCH_ITEM*        LocateAndShowItem( const wxPoint& aPosition, bool aIncludePin = true );
    SCH_ITEM*        LocateItem( const wxPoint& aPosition, bool aIncludePin );

    /**
     * Function FillFootprintFieldForAllInstancesofComponent
     * searches for component "aReference", and places a Footprint in
     * Footprint field
     * @param aReference = reference of the component to initialize
     * @param aFootPrint = new value for the filed Footprint component
     * @param aSetVisible = true to have the field visible, false to set the
     *                      invisible flag
     * @return true if the given component is found
     * Note:
     * the component is searched in the whole schematic, and because some
     * components have more than one instance (multiple parts per package
     *  components) the search is not stopped when a reference is found
     * (all instances must be found).
     */
    bool             FillFootprintFieldForAllInstancesofComponent( const wxString& aReference,
                                                                   const wxString& aFootPrint,
                                                                   bool            aSetVisible );

    SCH_ITEM*        FindComponentAndItem( const wxString& component_reference,
                                           bool            Find_in_hierarchy,
                                           int             SearchType,
                                           const wxString& text_to_find,
                                           bool            mouseWarp );

    /* Cross probing with pcbnew */
    void         SendMessageToPCBNEW( EDA_ITEM* objectToSync, SCH_COMPONENT*  LibItem );

    /* netlist generation */
    void         BuildNetListBase();

    /**
     * Function CreateNetlist
     * Create a netlist file:
     *  build netlist info
     *  test issues
     *  create file
     * @param aFormat = netlist format (NET_TYPE_PCBNEW ...)
     * @param aFullFileName = full netlist file name
     * @param aUse_netnames = bool. if true, use net names from labels in schematic
     *                              if false, use net numbers (net codes)
     *   bool aUse_netnames is used only for Spice netlist
     * @return true if success.
     */
    bool         CreateNetlist( int             aFormat,
                                const wxString& aFullFileName,
                                bool            aUse_netnames );

    /**
     * Function  WriteNetListFile
     * Create the netlist file. Netlist info must be existing
     * @param aFormat = netlist format (NET_TYPE_PCBNEW ...)
     * @param aFullFileName = full netlist file name
     * @param aUse_netnames = bool. if true, use net names from labels in schematic
     *                              if false, use net numbers (net codes)
     *   bool aUse_netnames is used only for Spice netlist
     * @return true if success.
     */
    bool         WriteNetListFile( int             aFormat,
                                   const wxString& aFullFileName,
                                   bool            aUse_netnames );

    /**
     * Function DeleteAnnotation
     * Remove current component annotations
     * @param aCurrentSheetOnly : if false: remove all annotations, else
     *                            remove annotation relative to the current
     *                            sheet only
     */
    void         DeleteAnnotation( bool aCurrentSheetOnly );

    /**
     * Function AnnotateComponents:
     *
     *  Compute the annotation of the components for the whole project, or the
     *  current sheet only.  All the components or the new ones only will be
     *  annotated.
     * @param aAnnotateSchematic : true = entire schematic annotation,
     *                            false = current sheet only
     * @param aSortOption : 0 = annotate by sorting X position,
     *                      1 = annotate by sorting Y position,
     *                      2 = annotate by sorting value
     * @param aAlgoOption : 0 = annotate schematic using first free Id number
     *                      1 = annotate using first free Id number, starting to sheet number * 100
     *                      2 = annotate  using first free Id number, starting to sheet number * 1000
     * @param aResetAnnotation : true = remove previous annotation
     *                          false = annotate new components only
     * @param aRepairsTimestamps : true = test for duplicate times stamps and
     *                                   replace duplicated
     *        Note: this option could change previous annotation, because time
     *              stamps are used to handle annotation mainly in complex
     *              hierarchies.
     * When the sheet number is used in annotation,
     *      for each sheet annotation starts from sheet number * 100
     *      ( the first sheet uses 100 to 199, the second 200 to 299 ... )
     */
    void AnnotateComponents( bool aAnnotateSchematic, int aSortOption, int aAlgoOption,
                             bool aResetAnnotation, bool aRepairsTimestamps );

    // Functions used for hierarchy handling
    /**
     * Function DisplayCurrentSheet
     * draws the current sheet on the display.
     */
    void         DisplayCurrentSheet();

    /**
     * Function GetUniqueFilenameForCurrentSheet
     * @return a filename that can be used in plot and print functions
     * for the current screen and sheet path.
     * This filename is unique and must be used instead of the screen filename
     * (or screen filename) when one must creates file for each sheet in the
     * hierarchy.  because in complex hierarchies a sheet and a SCH_SCREEN is
     * used more than once
     * Name is &ltroot sheet filename&gt-&ltsheet path&gt and has no extension.
     * However if filename is too long name is &ltsheet filename&gt-&ltsheet number&gt
     */
    wxString     GetUniqueFilenameForCurrentSheet();

    /**
     * Function SetSheetNumberAndCount
     * Set the m_ScreenNumber and m_NumberOfScreen members for screens
     * must be called after a delete or add sheet command, and when entering
     * a sheet
     */
    void         SetSheetNumberAndCount();

    /**
     * Show the print dialog
     */
    void         OnPrint( wxCommandEvent& event );

    wxPageSetupDialogData& GetPageSetupData() { return m_pageSetupData; }

    void             SetPreviewPosition( const wxPoint& aPoint ) { m_previewPosition = aPoint; }
    void             SetPreviewSize( const wxSize& aSize ) { m_previewSize = aSize; }
    const wxPoint& GetPreviewPosition() { return m_previewPosition; }
    const wxSize&  GetPreviewSize() { return m_previewSize; }

    void             SetPrintDialogPosition( const wxPoint& aPoint )
    {
        m_printDialogPosition = aPoint;
    }


    void             SetPrintDialogSize( const wxSize& aSize ) { m_printDialogSize = aSize; }
    const wxPoint& GetPrintDialogPosition() { return m_printDialogPosition; }
    const wxSize&  GetPrintDialogSize() { return m_printDialogSize; }

    bool             GetPrintMonochrome() { return m_printMonochrome; }
    void             SetPrintMonochrome( bool aMonochrome ) { m_printMonochrome = aMonochrome; }
    bool             GetPrintSheetReference() { return m_printSheetReference; }
    void             SetPrintSheetReference( bool aShow ) { m_printSheetReference = aShow; }
    void             SVG_Print( wxCommandEvent& event );

    // Plot functions:
    void            ToPlot_PS( wxCommandEvent& event );
    void            ToPlot_HPGL( wxCommandEvent& event );
    void            ToPlot_DXF( wxCommandEvent& event );
    void            ToPostProcess( wxCommandEvent& event );

    // read and save files
    void            Save_File( wxCommandEvent& event );
    void            SaveProject();
    bool            LoadOneEEProject( const wxString& FileName, bool IsNew );
    bool            LoadOneEEFile( SCH_SCREEN* screen, const wxString& FullFileName );
    bool            ReadInputStuffFile();

    /**
     * Function ProcessStuffFile
     * gets footprint info from each line in the Stuff File by Ref Desg
     *
     * Read a "stuff" file created by cvpcb.
     * That file has lines like:
     * comp = "C1" module = "CP6"
     * comp = "C2" module = "C1"
     * comp = "C3" module = "C1"
     * "comp =" gives the component reference
     * "module =" gives the footprint name
     *
     * @param aFilename The file to read from.
     * @param aSetFieldsAttributeToVisible = true to set the footprint field flag to visible
     * @return bool - true if success, else true.
     */
    bool            ProcessStuffFile( FILE* aFilename, bool  aSetFieldsAttributeToVisible );

    bool            SaveEEFile( SCH_SCREEN* screen, int FileSave );

    // General search:

private:
    void            Process_Move_Item( SCH_ITEM* DrawStruct, wxDC* DC );
    void            OnExit( wxCommandEvent& event );
    void            OnAnnotate( wxCommandEvent& event );
    void            OnErc( wxCommandEvent& event );
    void            OnCreateNetlist( wxCommandEvent& event );
    void            OnCreateBillOfMaterials( wxCommandEvent& event );
    void            OnFindItems( wxCommandEvent& event );
    void            OnFindDialogClose( wxFindDialogEvent& event );
    void            OnFindDrcMarker( wxFindDialogEvent& event );
    void            OnFindCompnentInLib( wxFindDialogEvent& event );
    void            OnFindSchematicItem( wxFindDialogEvent& event );
    void            OnLoadFile( wxCommandEvent& event );
    void            OnLoadStuffFile( wxCommandEvent& event );
    void            OnNewProject( wxCommandEvent& event );
    void            OnLoadProject( wxCommandEvent& event );
    void            OnOpenPcbnew( wxCommandEvent& event );
    void            OnOpenCvpcb( wxCommandEvent& event );
    void            OnOpenLibraryViewer( wxCommandEvent& event );
    void            OnOpenLibraryEditor( wxCommandEvent& event );
    void            OnSetOptions( wxCommandEvent& event );

    /* edition events functions */
    void            OnCopySchematicItemRequest( wxCommandEvent& event );

    /* User interface update event handlers. */
    void            OnUpdateBlockSelected( wxUpdateUIEvent& event );
    void            OnUpdatePaste( wxUpdateUIEvent& event );
    void            OnUpdateSchematicUndo( wxUpdateUIEvent& event );
    void            OnUpdateSchematicRedo( wxUpdateUIEvent& event );
    void            OnUpdateGrid( wxUpdateUIEvent& event );
    void            OnUpdateUnits( wxUpdateUIEvent& event );
    void            OnUpdateSelectCursor( wxUpdateUIEvent& event );
    void            OnUpdateHiddenPins( wxUpdateUIEvent& event );
    void            OnUpdateBusOrientation( wxUpdateUIEvent& event );

    /**
     * Function SetLanguage
     * called on a language menu selection
     */
    void            SetLanguage( wxCommandEvent& event );

    // Bus Entry
    SCH_BUS_ENTRY*  CreateBusEntry( wxDC* DC, int entry_type );
    void            SetBusEntryShape( wxDC*          DC,
                                      SCH_BUS_ENTRY* BusEntry,
                                      int            entry_type );
    int             GetBusEntryShape( SCH_BUS_ENTRY* BusEntry );
    void            StartMoveBusEntry( SCH_BUS_ENTRY* DrawLibItem,
                                       wxDC*          DC );

    /**
     * Function AddNoConnect
     * add a no connect item to the current schematic sheet at \a aPosition.
     * @param aDC The device context to draw the no connect to.
     * @param aPosition The position in logical (drawing) units to add the no connect.
     * @return The no connect item added.
     */
    SCH_NO_CONNECT* AddNoConnect( wxDC* aDC, const wxPoint& aPosition );

    // Junction
    SCH_JUNCTION*   AddJunction( wxDC* aDC, const wxPoint& aPosition, bool aPutInUndoList = FALSE );

    // Text ,label, glabel
    SCH_TEXT*       CreateNewText( wxDC* DC, int type );
    void            EditSchematicText( SCH_TEXT* TextStruct );
    void            ChangeTextOrient( SCH_TEXT* TextStruct, wxDC* DC );
    void            StartMoveTexte( SCH_TEXT* TextStruct, wxDC* DC );
    void            ConvertTextType( SCH_TEXT* Text, wxDC* DC, int newtype );

    // Wire, Bus
    void            BeginSegment( wxDC* DC, int type );
    void            EndSegment( wxDC* DC );
    void            DeleteCurrentSegment( wxDC* DC );
    void            DeleteConnection( bool DeleteFullConnection );

    // graphic lines
    void            Delete_Segment_Edge( DRAWSEGMENT* Segment, wxDC* DC );
    void            Drawing_SetNewWidth( DRAWSEGMENT* DrawSegm, wxDC* DC );
    void            Delete_Drawings_All_Layer( DRAWSEGMENT* Segment, wxDC* DC );
    DRAWSEGMENT*    Begin_Edge( DRAWSEGMENT* Segment, wxDC* DC );

    // Hierarchical Sheet & PinSheet
    void            InstallHierarchyFrame( wxDC* DC, wxPoint& pos );
    SCH_SHEET*      CreateSheet( wxDC* DC );
    void            ReSizeSheet( SCH_SHEET* Sheet, wxDC* DC );

    /**
     * Use the component viewer to select component to import into schematic.
     */
    wxString        SelectFromLibBrowser( void );

public:
    bool            EditSheet( SCH_SHEET* Sheet, wxDC* DC );

private:
    void            StartMoveSheet( SCH_SHEET* sheet, wxDC* DC );
    SCH_SHEET_PIN*  Create_PinSheet( SCH_SHEET* Sheet, wxDC* DC );
    int             Edit_PinSheet( SCH_SHEET_PIN* SheetLabel, wxDC* DC );
    void            StartMove_PinSheet( SCH_SHEET_PIN* SheetLabel,
                                        wxDC*          DC );
    void            Place_PinSheet( SCH_SHEET_PIN* SheetLabel,
                                    wxDC*          DC );
    SCH_SHEET_PIN*  Import_PinSheet( SCH_SHEET* Sheet, wxDC* DC );

public:
    void            DeleteSheetLabel( bool           aRedraw,
                                      SCH_SHEET_PIN* aSheetLabelToDel );

    int GetLabelIncrement() const { return m_repeatLabelDelta; }

private:

    // Component
    SCH_COMPONENT* Load_Component( wxDC*           DC,
                                   const wxString& libname,
                                   wxArrayString&  List,
                                   bool            UseLibBrowser );
    void           StartMovePart( SCH_COMPONENT* DrawLibItem, wxDC* DC );

public:
    void           CmpRotationMiroir( SCH_COMPONENT* DrawComponent,
                                      wxDC* DC, int type_rotate );

private:
    void           SelPartUnit( SCH_COMPONENT* DrawComponent,
                                int unit, wxDC* DC );
    void           ConvertPart( SCH_COMPONENT* DrawComponent, wxDC* DC );
    void           SetInitCmp( SCH_COMPONENT* DrawComponent, wxDC* DC );
    void           EditComponentReference( SCH_COMPONENT* DrawLibItem,
                                           wxDC*          DC );
    void           EditComponentValue( SCH_COMPONENT* DrawLibItem, wxDC* DC );
    void           EditComponentFootprint( SCH_COMPONENT* DrawLibItem,
                                           wxDC*          DC );
    void           StartMoveCmpField( SCH_FIELD* Field, wxDC* DC );
    void           EditCmpFieldText( SCH_FIELD* Field, wxDC* DC );
    void           RotateCmpField( SCH_FIELD* Field, wxDC* DC );

    void           PasteListOfItems( wxDC* DC );

    /* Undo - redo */
public:

    /**
     * Function SaveCopyInUndoList.
     * Create a copy of the current schematic item, and put it in the undo list.
     *
     *  flag_type_command =
     *      UR_CHANGED
     *      UR_NEW
     *      UR_DELETED
     *      UR_WIRE_IMAGE
     *      UR_MOVED
     *
     * If it is a delete command, items are put on list with the .Flags member
     * set to UR_DELETED.  When it will be really deleted, the GetDrawItems() and the
     * sub-hierarchy will be deleted.  If it is only a copy, the GetDrawItems() and the
     * sub-hierarchy must NOT be deleted.
     *
     * @note
     * Edit wires and buses is a bit complex.
     * because when a new wire is added, a lot of modifications in wire list is made
     * (wire concatenation): modified items, deleted items and new items
     * so flag_type_command is UR_WIRE_IMAGE: the struct ItemToCopy is a list of
     * wires saved in Undo List (for Undo or Redo commands, saved wires will be
     * exchanged with current wire list
     * @param aItemToCopy = the schematic item modified by the command to undo
     * @param aTypeCommand = command type (see enum UndoRedoOpType)
     * @param aTransformPoint = the reference point of the transformation,
     *                          for commands like move
     */
    void SaveCopyInUndoList( SCH_ITEM* aItemToCopy,
                             UndoRedoOpType aTypeCommand,
                             const wxPoint& aTransformPoint = wxPoint( 0, 0 ) );

    /**
     * Function SaveCopyInUndoList (overloaded).
     * Creates a new entry in undo list of commands.
     * add a list of pickers to handle a list of items
     * @param aItemsList = the list of items modified by the command to undo
     * @param aTypeCommand = command type (see enum UndoRedoOpType)
     * @param aTransformPoint = the reference point of the transformation,
     *                          for commands like move
     */
    void SaveCopyInUndoList( PICKED_ITEMS_LIST& aItemsList,
                             UndoRedoOpType aTypeCommand,
                             const wxPoint& aTransformPoint = wxPoint( 0, 0 ) );

private:

    /**
     * Function PutDataInPreviousState
     * is used in undo or redo command to put data pointed by List in the previous state, i.e.
     * the state stored in \a aList
     * @param aList a PICKED_ITEMS_LIST pointer to the list of items to undo/redo
     * @param aRedoCommand  a bool: true for redo, false for undo
     */
    void PutDataInPreviousState( PICKED_ITEMS_LIST* aList, bool aRedoCommand );

    /**
     * Function GetSchematicFromRedoList
     *  Redo the last edition:
     *  - Save the current schematic in Undo list
     *  - Get an old version of the schematic from Redo list
     *  @return none
     */
    void     GetSchematicFromRedoList( wxCommandEvent& event );

    /**
     * Function GetSchematicFromUndoList
     * performs an undo the last edition:
     *  - Save the current schematic in Redo list
     *  - Get an old version of the schematic from Undo list
     */
    void     GetSchematicFromUndoList( wxCommandEvent& event );

    /**
     * Function copyBlockItems
     * copies the list of block item.
     * @sa m_blockItems
     * @param aItemList List to copy the block select items into.
     */
    void copyBlockItems( PICKED_ITEMS_LIST& aItemsList );

public:
    void     Key( wxDC* DC, int hotkey, EDA_ITEM* DrawStruct );

    /* Block operations. */
    void     InitBlockPasteInfos();
    void     HandleBlockEndByPopUp( int Command, wxDC* DC );

    /**
     * Function ReturnBlockCommand
     * Returns the block command internat code (BLOCK_MOVE, BLOCK_COPY...)
     * corresponding to the keys pressed (ALT, SHIFT, SHIFT ALT ..) when
     * block command is started by dragging the mouse.
     * @param aKey = the key modifiers (Alt, Shift ...)
     * @return the block command id (BLOCK_MOVE, BLOCK_COPY...)
     */
    virtual int  ReturnBlockCommand( int aKey );

    /**
     * Function HandleBlockPlace
     * Called after HandleBlockEnd, when a block command needs to be
     * executed after the block is moved to its new place
     * (bloc move, drag, copy .. )
     * Parameters must be initialized in GetScreen()->m_BlockLocate
     */
    virtual void HandleBlockPlace( wxDC* DC );

    /**
     * Function HandleBlockEnd
     * Handle the "end"  of a block command,
     * i.e. is called at the end of the definition of the area of a block.
     * depending on the current block command, this command is executed
     * or parameters are initialized to prepare a call to HandleBlockPlace
     * in GetScreen()->m_BlockLocate
     * @return false if no item selected, or command finished,
     * true if some items found and HandleBlockPlace must be called later
     */
    virtual bool HandleBlockEnd( wxDC* DC );

    void     RepeatDrawItem( wxDC* DC );

    void SetRepeatItem( SCH_ITEM* aItem ) { m_itemToRepeat = aItem; }

    // ERC:

    /**
     * Function CheckAnnotate
     *  Check errors relatives to annotation:
     *      components not annotated
     *      components having the same reference (duplicates)
     *      for multiple parts per package components :
     *          part number > number of parts
     *          different values between parts
     * @return errors count
     * @param aMessageList = a wxArrayString to store messages. If NULL,
     *                       they are displayed in a wxMessageBox
     * @param aOneSheetOnly : true = search is made only in the current sheet
     *                       false = search in whole hierarchy (usual search).
     */
    int      CheckAnnotate( wxArrayString* aMessageList, bool aOneSheetOnly );

    /**
     * Load component libraries defined in project file.
     */
    void     LoadLibraries( void );


    DECLARE_EVENT_TABLE()
};


#endif  // WX_EESCHEMA_STRUCT_H
