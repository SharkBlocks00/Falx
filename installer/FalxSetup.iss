#define MyAppName "Falx"
#define MyAppVersion "0.1.0"
#define MyAppPublisher "Falx"
#define MyAppURL "https://github.com/sharkblocks00/Falx"
#define MyAppExeName "falx.exe"
#define MyAppDescription "The Falx programming language"
#define MyAppCopyright "Copyright (c) 2026 Falx contributors"

; ---------------------------------------------------------------------------
; Falx Programming Language
; Inno Setup installer
;
; Expected repository structure:
;
; Falx/
; ├── assets/
; │   └── falx.ico
; ├── dist/
; │   └── falx.exe
; └── installer/
;     └── falx.iss
;
; Build:
;   1. Build falx.exe with PyInstaller.
;   2. Open this file in Inno Setup.
;   3. Compile.
;
; ---------------------------------------------------------------------------

; ===========================================================================
; Application configuration
; ===========================================================================

[Setup]

; Unique application identifier.
; Keep this unchanged for future versions so Inno Setup recognises upgrades.
AppId={{8D7F3F3C-0F6D-4B5E-9D7A-FA1C8E5B0E21}

AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName} {#MyAppVersion}

AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}

AppCopyright={#MyAppCopyright}

; ---------------------------------------------------------------------------
; Installation directory
; ---------------------------------------------------------------------------

DefaultDirName={autopf}\Falx
DefaultGroupName=Falx

; Prevent installing into the Windows directory or other dangerous locations.
DisableDirPage=no

; ---------------------------------------------------------------------------
; Start Menu
; ---------------------------------------------------------------------------

DisableProgramGroupPage=no

; ---------------------------------------------------------------------------
; Output
; ---------------------------------------------------------------------------

OutputDir=output
OutputBaseFilename=Falx-Setup-{#MyAppVersion}

Compression=lzma2
SolidCompression=yes

; ---------------------------------------------------------------------------
; Installer appearance
; ---------------------------------------------------------------------------


UninstallDisplayIcon={app}{#MyAppExeName}

WizardStyle=modern

; Modern installer dimensions.
WizardSizePercent=100

; ---------------------------------------------------------------------------
; Privileges
; ---------------------------------------------------------------------------

; Installing into Program Files requires elevation.
PrivilegesRequired=admin

; ---------------------------------------------------------------------------
; Architecture
; ---------------------------------------------------------------------------

ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible

; ---------------------------------------------------------------------------
; Uninstallation
; ---------------------------------------------------------------------------

Uninstallable=yes
CreateUninstallRegKey=yes

; ---------------------------------------------------------------------------
; Miscellaneous
; ---------------------------------------------------------------------------

AllowNoIcons=yes
AllowRootDirectory=no

; Prevent accidental installation over another directory containing
; unrelated files.
DirExistsWarning=auto

; ---------------------------------------------------------------------------
; Version information embedded into the installer
; ---------------------------------------------------------------------------

VersionInfoVersion={#MyAppVersion}
VersionInfoCompany={#MyAppPublisher}
VersionInfoDescription=Falx Programming Language Installer
VersionInfoProductName=Falx
VersionInfoProductVersion={#MyAppVersion}
VersionInfoCopyright={#MyAppCopyright}

; ---------------------------------------------------------------------------
; Privilege / Windows behaviour
; ---------------------------------------------------------------------------

MinVersion=10.0

; ---------------------------------------------------------------------------
; Installer security / integrity
; ---------------------------------------------------------------------------

SignedUninstaller=no

; ===========================================================================
; Languages
; ===========================================================================

[Languages]

Name: "english"; MessagesFile: "compiler:Default.isl"

; ===========================================================================
; Tasks
; ===========================================================================

[Tasks]

; Start Menu shortcut.
Name: "startmenuicon"
Description: "Create a &Start Menu shortcut"
GroupDescription: "Shortcuts:"
Flags: unchecked

; Desktop shortcut.
Name: "desktopicon"
Description: "Create a &Desktop shortcut"
GroupDescription: "Shortcuts:"
Flags: unchecked

; Add Falx to PATH.
Name: "addtopath"
Description: "Add Falx to the system &PATH"
GroupDescription: "Environment:"
Flags: checkedonce

; ===========================================================================
; Files
; ===========================================================================

[Files]

; Main Falx executable.
Source: "..\dist\falx.exe"
DestDir: "{app}"
Flags: ignoreversion

; ===========================================================================
; Shortcuts
; ===========================================================================

[Icons]

; Start Menu shortcut.
Name: "{group}\Falx"
Filename: "{app}{#MyAppExeName}"
WorkingDir: "{app}"
IconFilename: "{app}{#MyAppExeName}"
Comment: "Run the Falx programming language"
Tasks: startmenuicon

; Start Menu documentation shortcut.
Name: "{group}\Falx Documentation"
Filename: "{#MyAppURL}"
Comment: "Open Falx documentation"
Tasks: startmenuicon

; Desktop shortcut.
Name: "{autodesktop}\Falx"
Filename: "{app}{#MyAppExeName}"
WorkingDir: "{app}"
IconFilename: "{app}{#MyAppExeName}"
Comment: "Run the Falx programming language"
Tasks: desktopicon

; ===========================================================================
; Registry
; ===========================================================================

[Registry]

; ---------------------------------------------------------------------------
; Store the installation directory.
; ---------------------------------------------------------------------------

Root: HKLM
Subkey: "Software\Falx"
ValueType: string
ValueName: "InstallPath"
ValueData: "{app}"
Flags: uninsdeletekey

; ---------------------------------------------------------------------------
; Store the installed version.
; ---------------------------------------------------------------------------

Root: HKLM
Subkey: "Software\Falx"
ValueType: string
ValueName: "Version"
ValueData: "{#MyAppVersion}"

; ---------------------------------------------------------------------------
; Optional PATH entry.
;
; This is intentionally NOT blindly appended using a normal registry entry.
; PATH modification is handled by Pascal Script below so duplicate entries
; can be detected and avoided.
; ---------------------------------------------------------------------------

; ===========================================================================
; Uninstall cleanup
; ===========================================================================

[UninstallDelete]

; Remove empty installation directories after uninstall.
Type: dirifempty
Name: "{app}"

; ===========================================================================
; Pascal Script
; ===========================================================================
;
; The following code handles PATH management.
;
; Requirements:
;
;   - Do not add Falx if it is already present.
;   - Do not add duplicate Falx entries.
;   - Preserve all existing PATH entries.
;   - Remove Falx cleanly during uninstall.
;   - Handle both normal and trailing-slash versions.
;   - Handle quoted PATH entries.
;   - Handle case-insensitive Windows paths.
;
; ===========================================================================

[Code]

const
EnvironmentKey = 'SYSTEM\CurrentControlSet\Control\Session Manager\Environment';
FalxPathRegistryKey = 'Software\Falx';
FalxPathRegistryValue = 'PathAdded';

function NormalizePathEntry(PathEntry: string): string;
begin
Result := Trim(PathEntry);

{ Remove surrounding quotes. }
if (Length(Result) >= 2) and
(Result[1] = '"') and
(Result[Length(Result)] = '"') then
begin
Result := Copy(Result, 2, Length(Result) - 2);
end;

{ Remove trailing path separators. }
while (Length(Result) > 3) and
((Result[Length(Result)] = '') or
(Result[Length(Result)] = '/')) do
begin
Delete(Result, Length(Result), 1);
end;
end;

function PathEntryEquals(PathA, PathB: string): Boolean;
begin
Result :=
CompareText(
NormalizePathEntry(PathA),
NormalizePathEntry(PathB)
) = 0;
end;

function SplitPath(const PathValue: string; var Entries: array of string): Integer;
var
CurrentEntry: string;
Index: Integer;
EntryCount: Integer;
begin
EntryCount := 0;
CurrentEntry := '';

for Index := 1 to Length(PathValue) do
begin
if PathValue[Index] = ';' then
begin
if Trim(CurrentEntry) <> '' then
begin
if EntryCount < GetArrayLength(Entries) then
begin
Entries[EntryCount] := CurrentEntry;
Inc(EntryCount);
end;
end;


  CurrentEntry := '';
end
else
begin
  CurrentEntry := CurrentEntry + PathValue[Index];
end;

end;

if Trim(CurrentEntry) <> '' then
begin
if EntryCount < GetArrayLength(Entries) then
begin
Entries[EntryCount] := CurrentEntry;
Inc(EntryCount);
end;
end;

Result := EntryCount;
end;

function PathContainsEntry(PathValue, EntryToFind: string): Boolean;
var
Entries: array[0..511] of string;
EntryCount: Integer;
Index: Integer;
begin
Result := False;

EntryCount := SplitPath(PathValue, Entries);

for Index := 0 to EntryCount - 1 do
begin
if PathEntryEquals(Entries[Index], EntryToFind) then
begin
Result := True;
Exit;
end;
end;
end;

function RemovePathEntry(PathValue, EntryToRemove: string): string;
var
Entries: array[0..511] of string;
EntryCount: Integer;
Index: Integer;
NewPath: string;
begin
NewPath := '';

EntryCount := SplitPath(PathValue, Entries);

for Index := 0 to EntryCount - 1 do
begin
if not PathEntryEquals(Entries[Index], EntryToRemove) then
begin
if NewPath <> '' then
NewPath := NewPath + ';';


  NewPath := NewPath + Entries[Index];
end;


end;

Result := NewPath;
end;

procedure BroadcastEnvironmentChange;
var
ResultCode: Integer;
begin
{ Notify running applications that the environment has changed. }
SendMessage(
HWND_BROADCAST,
WM_SETTINGCHANGE,
0,
0
);
end;

procedure AddFalxToPath;
var
CurrentPath: string;
FalxDirectory: string;
NewPath: string;
begin
FalxDirectory := ExpandConstant('{app}');

if not RegQueryStringValue(
HKEY_LOCAL_MACHINE,
EnvironmentKey,
'Path',
CurrentPath
) then
begin
CurrentPath := '';
end;

{ Do nothing if Falx is already present. }
if PathContainsEntry(CurrentPath, FalxDirectory) then
begin
RegWriteStringValue(
HKEY_LOCAL_MACHINE,
FalxPathRegistryKey,
FalxPathRegistryValue,
'1'
);


Exit;

end;

if CurrentPath = '' then
NewPath := FalxDirectory
else
NewPath := CurrentPath + ';' + FalxDirectory;

RegWriteExpandStringValue(
HKEY_LOCAL_MACHINE,
EnvironmentKey,
'Path',
NewPath
);

RegWriteStringValue(
HKEY_LOCAL_MACHINE,
FalxPathRegistryKey,
FalxPathRegistryValue,
'1'
);

BroadcastEnvironmentChange;
end;

procedure RemoveFalxFromPath;
var
CurrentPath: string;
FalxDirectory: string;
NewPath: string;
begin
FalxDirectory := ExpandConstant('{app}');

if not RegQueryStringValue(
HKEY_LOCAL_MACHINE,
EnvironmentKey,
'Path',
CurrentPath
) then
begin
Exit;
end;

NewPath := RemovePathEntry(
CurrentPath,
FalxDirectory
);

if CompareText(NewPath, CurrentPath) <> 0 then
begin
RegWriteExpandStringValue(
HKEY_LOCAL_MACHINE,
EnvironmentKey,
'Path',
NewPath
);


BroadcastEnvironmentChange;

end;

RegDeleteKeyIfEmpty(
HKEY_LOCAL_MACHINE,
FalxPathRegistryKey
);
end;

procedure CurStepChanged(CurStep: TSetupStep);
begin
if CurStep = ssPostInstall then
begin
if WizardIsTaskSelected('addtopath') then
begin
AddFalxToPath;
end;
end;
end;

procedure CurUninstallStepChanged(
UninstallStep: TUninstallStep
);
begin
if UninstallStep = usUninstall then
begin
RemoveFalxFromPath;
end;
end;

function NextButtonClick(
CurPageID: Integer
): Boolean;
begin
Result := True;

{ Allow the normal wizard flow to continue. }
end;

function PrepareToInstall(
NeedsRestart: Boolean
): String;
begin
Result := '';

{ Verify that the executable exists before installation starts. }

if not FileExists(
ExpandConstant('{src}..\dist\falx.exe')
) then
begin
Result :=
'Falx executable was not found.' + #13#10#13#10 +
'Expected:' + #13#10 +
ExpandConstant('{src}..\dist\falx.exe') + #13#10#13#10 +
'Build Falx with PyInstaller before building the installer.';
end;
end;
