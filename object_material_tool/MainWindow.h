#pragma once

// c++
#include <Windows.h>

// current program
#include "Lines.h"
#include "ObjectDecompiler.h"
#include "About.h"
#include "resource.h"

// libs
#pragma comment(lib, "user32.lib")

namespace object_material_tool 
{

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// ������ ��� MainWindow
	/// </summary>
	public ref class MainWindow : public System::Windows::Forms::Form
	{
	public:
		MainWindow(void)
		{
			InitializeComponent();
			this->init();
		}

	protected:
		/// <summary>
		/// ���������� ��� ������������ �������.
		/// </summary>
		~MainWindow()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^  MainMenu;
	private: System::Windows::Forms::ToolStripMenuItem^  MainMenuFile;
	private: System::Windows::Forms::ToolStripMenuItem^  MainMenuFileReadObject;
	private: System::Windows::Forms::ToolStripMenuItem^  MainMenuFileDecompileObject;
	private: System::Windows::Forms::ToolStripMenuItem^  MainMenuEdit;
	private: System::Windows::Forms::ToolStripMenuItem^  MainMenuEditClearLog;
	private: System::Windows::Forms::ToolStripMenuItem^  MainMenuEditCopySelectedTextFromLog;
	private: System::Windows::Forms::ToolStripMenuItem^  MainMenuEditCopyAllFromLog;
	private: System::Windows::Forms::ToolStripMenuItem^  MainMenuTools;
	private: System::Windows::Forms::ToolStripMenuItem^  MainMenuToolsGMobject;
	private: System::Windows::Forms::ToolStripMenuItem^  MainMenuToolsLODObject;

	private: System::Windows::Forms::ToolStripMenuItem^  MainMenuToolsHOMObject;
	private: System::Windows::Forms::ToolStripMenuItem^  MainMenuToolsAllCatalogueObjects;
	private: System::Windows::Forms::ToolStripMenuItem^  MainMenuDocs;
	private: System::Windows::Forms::ToolStripMenuItem^  MainMenuAbout;
	private: System::Windows::Forms::ToolStripMenuItem^  MainMenuExit;
	private: System::Windows::Forms::SplitContainer^  SplitPanel;
	private: System::Windows::Forms::Label^  Info;
	private: System::Windows::Forms::RichTextBox^  Log;
	private: System::Windows::Forms::ToolTip^  ToolTipAll;
	private: System::Windows::Forms::OpenFileDialog^  FilesOpen;
	private: System::Windows::Forms::SaveFileDialog^  FilesSave;
	private: System::Windows::Forms::ContextMenuStrip^  ContextLog;
	private: System::Windows::Forms::ToolStripMenuItem^  ContextLogCopy;
	private: System::Windows::Forms::ToolStripMenuItem^  ContextLogCopyAll;
	private: System::Windows::Forms::FolderBrowserDialog^  folderSelect;
	private: System::ComponentModel::IContainer^  components;




	protected:

	private:
		/// <summary>
		/// ������������ ���������� ������������.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// ��������� ����� ��� ��������� ������������ � �� ��������� 
		/// ���������� ����� ������ � ������� ��������� ����.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MainWindow::typeid));
			this->MainMenu = (gcnew System::Windows::Forms::MenuStrip());
			this->MainMenuFile = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->MainMenuFileReadObject = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->MainMenuFileDecompileObject = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->MainMenuEdit = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->MainMenuEditClearLog = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->MainMenuEditCopySelectedTextFromLog = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->MainMenuEditCopyAllFromLog = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->MainMenuTools = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->MainMenuToolsGMobject = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->MainMenuToolsLODObject = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->MainMenuToolsHOMObject = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->MainMenuToolsAllCatalogueObjects = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->MainMenuDocs = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->MainMenuAbout = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->MainMenuExit = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->SplitPanel = (gcnew System::Windows::Forms::SplitContainer());
			this->Log = (gcnew System::Windows::Forms::RichTextBox());
			this->ContextLog = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->ContextLogCopy = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ContextLogCopyAll = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->Info = (gcnew System::Windows::Forms::Label());
			this->ToolTipAll = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->FilesOpen = (gcnew System::Windows::Forms::OpenFileDialog());
			this->FilesSave = (gcnew System::Windows::Forms::SaveFileDialog());
			this->folderSelect = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->MainMenu->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->SplitPanel))->BeginInit();
			this->SplitPanel->Panel1->SuspendLayout();
			this->SplitPanel->Panel2->SuspendLayout();
			this->SplitPanel->SuspendLayout();
			this->ContextLog->SuspendLayout();
			this->SuspendLayout();
			// 
			// MainMenu
			// 
			this->MainMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {
				this->MainMenuFile, this->MainMenuEdit,
					this->MainMenuTools, this->MainMenuDocs, this->MainMenuAbout, this->MainMenuExit
			});
			this->MainMenu->Location = System::Drawing::Point(0, 0);
			this->MainMenu->Name = L"MainMenu";
			this->MainMenu->Size = System::Drawing::Size(487, 24);
			this->MainMenu->TabIndex = 0;
			this->MainMenu->Text = L"menuStrip1";
			// 
			// MainMenuFile
			// 
			this->MainMenuFile->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->MainMenuFileReadObject,
					this->MainMenuFileDecompileObject
			});
			this->MainMenuFile->Name = L"MainMenuFile";
			this->MainMenuFile->Size = System::Drawing::Size(48, 20);
			this->MainMenuFile->Text = L"����";
			// 
			// MainMenuFileReadObject
			// 
			this->MainMenuFileReadObject->Name = L"MainMenuFileReadObject";
			this->MainMenuFileReadObject->Size = System::Drawing::Size(185, 22);
			this->MainMenuFileReadObject->Text = L"���������� *.object";
			this->MainMenuFileReadObject->ToolTipText = L"����� ������� ���������� �� *.object";
			this->MainMenuFileReadObject->Click += gcnew System::EventHandler(this, &MainWindow::MainMenuFileReadObject_Click);
			// 
			// MainMenuFileDecompileObject
			// 
			this->MainMenuFileDecompileObject->Name = L"MainMenuFileDecompileObject";
			this->MainMenuFileDecompileObject->Size = System::Drawing::Size(185, 22);
			this->MainMenuFileDecompileObject->Text = L"��������� *.object";
			this->MainMenuFileDecompileObject->ToolTipText = L"������ *.object � �������� ����� ��������";
			this->MainMenuFileDecompileObject->Click += gcnew System::EventHandler(this, &MainWindow::MainMenuFileDecompileObject_Click);
			// 
			// MainMenuEdit
			// 
			this->MainMenuEdit->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->MainMenuEditClearLog,
					this->MainMenuEditCopySelectedTextFromLog, this->MainMenuEditCopyAllFromLog
			});
			this->MainMenuEdit->Name = L"MainMenuEdit";
			this->MainMenuEdit->Size = System::Drawing::Size(99, 20);
			this->MainMenuEdit->Text = L"�������������";
			// 
			// MainMenuEditClearLog
			// 
			this->MainMenuEditClearLog->Name = L"MainMenuEditClearLog";
			this->MainMenuEditClearLog->Size = System::Drawing::Size(209, 22);
			this->MainMenuEditClearLog->Text = L"�������� ���";
			this->MainMenuEditClearLog->Click += gcnew System::EventHandler(this, &MainWindow::MainMenuEditClearLog_Click);
			// 
			// MainMenuEditCopySelectedTextFromLog
			// 
			this->MainMenuEditCopySelectedTextFromLog->Name = L"MainMenuEditCopySelectedTextFromLog";
			this->MainMenuEditCopySelectedTextFromLog->Size = System::Drawing::Size(209, 22);
			this->MainMenuEditCopySelectedTextFromLog->Text = L"���������� ����������";
			this->MainMenuEditCopySelectedTextFromLog->Click += gcnew System::EventHandler(this, &MainWindow::MainMenuEditCopySelectedTextFromLog_Click);
			// 
			// MainMenuEditCopyAllFromLog
			// 
			this->MainMenuEditCopyAllFromLog->Name = L"MainMenuEditCopyAllFromLog";
			this->MainMenuEditCopyAllFromLog->Size = System::Drawing::Size(209, 22);
			this->MainMenuEditCopyAllFromLog->Text = L"���������� ��";
			this->MainMenuEditCopyAllFromLog->Click += gcnew System::EventHandler(this, &MainWindow::MainMenuEditCopyAllFromLog_Click);
			// 
			// MainMenuTools
			// 
			this->MainMenuTools->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
				this->MainMenuToolsGMobject,
					this->MainMenuToolsLODObject, this->MainMenuToolsHOMObject, this->MainMenuToolsAllCatalogueObjects
			});
			this->MainMenuTools->Name = L"MainMenuTools";
			this->MainMenuTools->Size = System::Drawing::Size(95, 20);
			this->MainMenuTools->Text = L"�����������";
			// 
			// MainMenuToolsGMobject
			// 
			this->MainMenuToolsGMobject->Name = L"MainMenuToolsGMobject";
			this->MainMenuToolsGMobject->Size = System::Drawing::Size(229, 22);
			this->MainMenuToolsGMobject->Text = L"��������� GM *object";
			this->MainMenuToolsGMobject->ToolTipText = L"��������� ��������� *.object";
			this->MainMenuToolsGMobject->Click += gcnew System::EventHandler(this, &MainWindow::MainMenuToolsGMobject_Click);
			// 
			// MainMenuToolsLODObject
			// 
			this->MainMenuToolsLODObject->Name = L"MainMenuToolsLODObject";
			this->MainMenuToolsLODObject->Size = System::Drawing::Size(229, 22);
			this->MainMenuToolsLODObject->Text = L"��������� LOD *object";
			this->MainMenuToolsLODObject->ToolTipText = L"��������� LOD *.object";
			this->MainMenuToolsLODObject->Click += gcnew System::EventHandler(this, &MainWindow::MainMenuToolsLODObject_Click);
			// 
			// MainMenuToolsHOMObject
			// 
			this->MainMenuToolsHOMObject->Name = L"MainMenuToolsHOMObject";
			this->MainMenuToolsHOMObject->Size = System::Drawing::Size(229, 22);
			this->MainMenuToolsHOMObject->Text = L"��������� HOM *.object";
			this->MainMenuToolsHOMObject->ToolTipText = L"��������� HOM *.object";
			this->MainMenuToolsHOMObject->Click += gcnew System::EventHandler(this, &MainWindow::MainMenuToolsHOMObject_Click);
			// 
			// MainMenuToolsAllCatalogueObjects
			// 
			this->MainMenuToolsAllCatalogueObjects->Name = L"MainMenuToolsAllCatalogueObjects";
			this->MainMenuToolsAllCatalogueObjects->Size = System::Drawing::Size(229, 22);
			this->MainMenuToolsAllCatalogueObjects->Text = L"��������� ������� *.object\'s";
			this->MainMenuToolsAllCatalogueObjects->ToolTipText = L"�������� ��������� *.objects\'s";
			this->MainMenuToolsAllCatalogueObjects->Click += gcnew System::EventHandler(this, &MainWindow::MainMenuToolsAllCatalogueObjects_Click);
			// 
			// MainMenuDocs
			// 
			this->MainMenuDocs->Name = L"MainMenuDocs";
			this->MainMenuDocs->Size = System::Drawing::Size(85, 20);
			this->MainMenuDocs->Text = L"����������";
			this->MainMenuDocs->Click += gcnew System::EventHandler(this, &MainWindow::MainMenuDocs_Click);
			// 
			// MainMenuAbout
			// 
			this->MainMenuAbout->Name = L"MainMenuAbout";
			this->MainMenuAbout->Size = System::Drawing::Size(94, 20);
			this->MainMenuAbout->Text = L"� ���������";
			this->MainMenuAbout->Click += gcnew System::EventHandler(this, &MainWindow::MainMenuAbout_Click);
			// 
			// MainMenuExit
			// 
			this->MainMenuExit->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
			this->MainMenuExit->Name = L"MainMenuExit";
			this->MainMenuExit->Size = System::Drawing::Size(53, 20);
			this->MainMenuExit->Text = L"�����";
			this->MainMenuExit->Click += gcnew System::EventHandler(this, &MainWindow::MainMenuExit_Click);
			// 
			// SplitPanel
			// 
			this->SplitPanel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->SplitPanel->IsSplitterFixed = true;
			this->SplitPanel->Location = System::Drawing::Point(0, 24);
			this->SplitPanel->Name = L"SplitPanel";
			this->SplitPanel->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// SplitPanel.Panel1
			// 
			this->SplitPanel->Panel1->Controls->Add(this->Log);
			// 
			// SplitPanel.Panel2
			// 
			this->SplitPanel->Panel2->Controls->Add(this->Info);
			this->SplitPanel->Size = System::Drawing::Size(487, 404);
			this->SplitPanel->SplitterDistance = 369;
			this->SplitPanel->TabIndex = 1;
			// 
			// Log
			// 
			this->Log->BackColor = System::Drawing::SystemColors::Window;
			this->Log->ContextMenuStrip = this->ContextLog;
			this->Log->Dock = System::Windows::Forms::DockStyle::Fill;
			this->Log->Location = System::Drawing::Point(0, 0);
			this->Log->Name = L"Log";
			this->Log->ReadOnly = true;
			this->Log->Size = System::Drawing::Size(487, 369);
			this->Log->TabIndex = 0;
			this->Log->Text = L"";
			this->Log->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainWindow::Log_MouseDown);
			this->Log->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MainWindow::Log_MouseMove);
			// 
			// ContextLog
			// 
			this->ContextLog->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) { this->ContextLogCopy, this->ContextLogCopyAll });
			this->ContextLog->Name = L"ContextLog";
			this->ContextLog->Size = System::Drawing::Size(161, 48);
			// 
			// ContextLogCopy
			// 
			this->ContextLogCopy->Name = L"ContextLogCopy";
			this->ContextLogCopy->Size = System::Drawing::Size(160, 22);
			this->ContextLogCopy->Text = L"����������";
			this->ContextLogCopy->Click += gcnew System::EventHandler(this, &MainWindow::ContextLogCopy_Click);
			// 
			// ContextLogCopyAll
			// 
			this->ContextLogCopyAll->Name = L"ContextLogCopyAll";
			this->ContextLogCopyAll->Size = System::Drawing::Size(160, 22);
			this->ContextLogCopyAll->Text = L"���������� ��";
			this->ContextLogCopyAll->Click += gcnew System::EventHandler(this, &MainWindow::ContextLogCopyAll_Click);
			// 
			// Info
			// 
			this->Info->AutoSize = true;
			this->Info->Location = System::Drawing::Point(3, 7);
			this->Info->Name = L"Info";
			this->Info->Size = System::Drawing::Size(132, 13);
			this->Info->TabIndex = 0;
			this->Info->Text = L"��������� ���������...";
			// 
			// FilesOpen
			// 
			this->FilesOpen->Filter = L"Stalker Objects|*.object";
			this->FilesOpen->RestoreDirectory = true;
			// 
			// FilesSave
			// 
			this->FilesSave->Filter = L"Text Files|*.txt";
			// 
			// MainWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::Window;
			this->ClientSize = System::Drawing::Size(487, 428);
			this->Controls->Add(this->SplitPanel);
			this->Controls->Add(this->MainMenu);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->MainMenuStrip = this->MainMenu;
			this->MinimumSize = System::Drawing::Size(503, 444);
			this->Name = L"MainWindow";
			this->SizeGripStyle = System::Windows::Forms::SizeGripStyle::Hide;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"ObjectDecompiler.NET v 2.0";
			this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MainWindow::MainWindow_Paint);
			this->Resize += gcnew System::EventHandler(this, &MainWindow::MainWindow_Resize);
			this->MainMenu->ResumeLayout(false);
			this->MainMenu->PerformLayout();
			this->SplitPanel->Panel1->ResumeLayout(false);
			this->SplitPanel->Panel2->ResumeLayout(false);
			this->SplitPanel->Panel2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->SplitPanel))->EndInit();
			this->SplitPanel->ResumeLayout(false);
			this->ContextLog->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: void init()
	{
		// ����������
		// ��������
		// �������������� ���������� ����������. ��������� ����������� ��������� �������������(�� ��������� ����������������� �����) ��� ���������� ������� ��� �������

		char msg[500];
		int error = ObjectDec::ini_parse(NULL);
		if (error < 0)
		{
			sprintf(msg, "Can't read '%s'!\r\n", ObjectDec::inifile);
			this->Log->Text = gcnew String(msg);
			//        return 2;
		}
		else if (error)
		{
			sprintf(msg, "Bad config file (first error on line %d)!\r\n", error);
			this->Log->Text = gcnew String(msg);
			//        return 3;
		}
		else
		{
			sprintf(msg, "��������� ����� %d �� INI �����.\r\n\r\n", ObjectDec::ini_count);
			this->Log->AppendText(gcnew String(msg));
			for (int i = 0; i < ObjectDec::ini_count; ++i)
			{
				sprintf(msg, "%s = %s | %s | %s | 0x%02X\r\n", ObjectDec::ini_sting[i].t_path, ObjectDec::ini_sting[i].e_shader, ObjectDec::ini_sting[i].c_shader, ObjectDec::ini_sting[i].g_material, ObjectDec::ini_sting[i].flags);
				this->Log->AppendText(gcnew String(msg));
			}
		}
	}

	private: System::Void MainWindow_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e)
	{
		HideCaret((HWND)this->Log->Handle.ToPointer());
	}
	private: System::Void Log_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		HideCaret((HWND)this->Log->Handle.ToPointer());
	}
	private: System::Void Log_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		HideCaret((HWND)this->Log->Handle.ToPointer());
	}
	private: System::Void MainMenuFileReadObject_Click(System::Object^  sender, System::EventArgs^  e)
	{
		this->Info->Text = "��������� �������...";
		this->FilesOpen->Title = "������� Object ...";
		if (this->FilesOpen->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			String^ file = this->FilesOpen->FileName;
			if (file != nullptr)
			{
				String^ fn = file->IndexOf("\\") != 0 ? file->Substring(file->LastIndexOf("\\") + 1) : file;
				this->Info->Text = "��������� ������ " + fn;

				std::string _file = Lines::systemToStdString(file);
				ObjectDec objdec(_file.c_str());
				this->Log->Clear();
				if (objdec.stlk_object_mtpatch(this->Log, 0))
				{
					this->Info->Text = "Error!";
					MessageBox::Show("������ ���������.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				}
				else
					this->Info->Text = "������ " + fn + " ������� ��������...";
				return;
			}
		}
		this->Info->Text = "������ ��������...";
	}
	private: System::Void MainMenuFileDecompileObject_Click(System::Object^  sender, System::EventArgs^  e)
	{
		this->Info->Text = "��������� �������...";
		this->FilesOpen->Title = "�������� ���� *.object ...";
		if (this->FilesOpen->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			String^ file = this->FilesOpen->FileName;
			if (file != nullptr)
			{
				String^ fn = file->IndexOf("\\") != 0 ? file->Substring(file->LastIndexOf("\\") + 1) : file;

				this->FilesSave->Title = "���� ��������� ����������� " + fn;
				fn = fn->Remove(fn->LastIndexOf(".")) + ".txt";
				this->FilesSave->FileName = fn;
				String^ _file;
				if (this->FilesSave->ShowDialog() == System::Windows::Forms::DialogResult::OK)
					_file = this->FilesSave->FileName;
				else
				{
					this->Info->Text = "������ ����������...";
					return;
				}
				if (file != nullptr)
				{
					this->Info->Text = "��������� ������� ����� " + fn + "...";
					std::string in = Lines::systemToStdString(file);
					std::string out = Lines::systemToStdString(_file);
					this->Log->Clear();
					ObjectDec objDec(in.c_str());
					if (objDec.stlk_object_parce(this->Log, out.c_str()))
					{
						this->Info->Text = "Error!";
						MessageBox::Show("������ ���������.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
					}
					else
						this->Info->Text = "������ " + fn + " ������� ��������...";
					return;
				}
				else
				{
					this->Info->Text = "������ ��������...";
					return;
				}
			}
			return;
		}
		this->Info->Text = "������ ����������...";
	}
	private: System::Void MainWindow_Resize(System::Object^  sender, System::EventArgs^  e)
	{
		Control^ control = dynamic_cast<Control^>(sender);
		this->Info->Text = "Resize: [" + control->Width + "/" + control->Height + "]";
	}

	private: System::Void MainMenuEditClearLog_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (this->Log->Text->Length != 0)
			this->Log->Clear();
	}
	private: System::Void MainMenuEditCopySelectedTextFromLog_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (this->Log->SelectedText->Length != 0)
			Clipboard::SetText(this->Log->SelectedText);
	}
	private: System::Void MainMenuEditCopyAllFromLog_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (this->Log->Text->Length != 0)
			Clipboard::SetText(this->Log->Text);
	}
	private: System::Void ContextLogCopy_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (this->Log->SelectedText->Length != 0)
			Clipboard::SetText(this->Log->SelectedText);
	}
	private: System::Void ContextLogCopyAll_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (this->Log->Text->Length != 0)
			Clipboard::SetText(this->Log->Text);
	}
	private: System::Void MainMenuExit_Click(System::Object^  sender, System::EventArgs^  e)
	{
		this->Close();
	}
	
	private: System::Void MainMenuToolsGMobject_Click(System::Object^  sender, System::EventArgs^  e)
	{
		this->Info->Text = "��������� �������...";
		this->FilesOpen->Title = "�������� ���� *.object ...";
		if (this->FilesOpen->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			String^ file = this->FilesOpen->FileName;
			if (file != nullptr)
			{
				String^ fn = file->IndexOf("\\") != 0 ? file->Substring(file->LastIndexOf("\\") + 1) : file; // ��� ���������� �����
				String^ file_old = file->Insert(file->LastIndexOf("."), "_oldest"); // ��� ������� �����
				String^ _log = fn->Substring(0, fn->LastIndexOf(".")) + ".txt"; // ��� ����

				this->Info->Text = "��������� ������ " + fn;

				// ���� ��������� ���
				this->FilesSave->Title = "���� ��������� ���: ";
				this->FilesSave->FileName = _log;
				String^ _file;
				if (this->FilesSave->ShowDialog() == System::Windows::Forms::DialogResult::OK)
					_file = this->FilesSave->FileName; // ���� �� ���� ���������!!!
				else
					this->Info->Text = "������ ���������� ����...";

				// ��������� ��������
				this->Log->Clear();
				ObjectDec objdec(Lines::systemToStdString(file_old).c_str());

				std::ofstream log;
				if(_file && file != "")
					log.open(Lines::systemToStdString(_file));
				if (objdec.stlk_object_gmpatch(this->Log, Lines::systemToStdString(file).c_str(), log, 0))
				{
					this->Info->Text = "Error!";
					MessageBox::Show("������ ���������.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				}
				else
					this->Info->Text = "��������� " + fn + " ������� ���������...";
				return;
			}
		}
		this->Info->Text = "������ ��������...";
	}
	private: System::Void MainMenuToolsLODObject_Click(System::Object^  sender, System::EventArgs^  e)
	{
		this->Info->Text = "��������� �������...";
		this->FilesOpen->Title = "������� Object ...";
		if (this->FilesOpen->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			String^ file = this->FilesOpen->FileName;
			if (file != nullptr)
			{
				String^ fn = file->IndexOf("\\") != 0 ? file->Substring(file->LastIndexOf("\\") + 1) : file;
				this->Info->Text = "��������� ������ " + fn;

				std::string _file = Lines::systemToStdString(file);
				ObjectDec objdec(_file.c_str());
				this->Log->Clear();
				if (objdec.stlk_object_mtpatch(this->Log, 1))
				{
					this->Info->Text = "Error!";
					MessageBox::Show("������ ���������.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				}
				else
					this->Info->Text = "��������� " + fn + " ������� ���������...";
				return;
			}
		}
		this->Info->Text = "������ ��������...";
	}
	private: System::Void MainMenuToolsHOMObject_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		this->Info->Text = "��������� �������...";
		this->FilesOpen->Title = "������� Object ...";
		if (this->FilesOpen->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			String^ file = this->FilesOpen->FileName;
			if (file != nullptr)
			{
				String^ fn = file->IndexOf("\\") != 0 ? file->Substring(file->LastIndexOf("\\") + 1) : file;
				this->Info->Text = "��������� ������ " + fn;

				std::string _file = Lines::systemToStdString(file);
				ObjectDec objdec(_file.c_str());
				this->Log->Clear();
				if (objdec.stlk_object_mtpatch(this->Log, 2))
				{
					this->Info->Text = "Error!";
					MessageBox::Show("������ ���������.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				}
				else
					this->Info->Text = "��������� " + fn + " ������� ���������...";
				return;
			}
		}
		this->Info->Text = "������ ��������...";
	}
	private: System::Void MainMenuToolsAllCatalogueObjects_Click(System::Object^  sender, System::EventArgs^  e)
	{
		String^ in_catalogue;
		String^ out_catalogue;
		String^ _file;

		this->folderSelect->Description = "�������� �������, ��� ����� *.object's...";
		if (this->folderSelect->ShowDialog() != System::Windows::Forms::DialogResult::OK)
		{
			this->Info->Text = "������ ��������...";
			return; // �� ������� ����� � ���������
		}
		else
			in_catalogue = this->folderSelect->SelectedPath;
		this->folderSelect->Description = "�������� �������, ���� �������� ����������� *.object's...";
		if (this->folderSelect->ShowDialog() != System::Windows::Forms::DialogResult::OK)
		{
			this->Info->Text = "������ ��������...";
			return; // �� ������� ����� ��� ��������
		}
		else
			out_catalogue = this->folderSelect->SelectedPath;

		char logFileName[500];
		time_t log_time;
		time(&log_time);
		sprintf(logFileName, "outlog_%d", (int)log_time);
		strcat(logFileName, ".log");
		this->FilesSave->Title = "���� ��������� ��� ...";
		this->FilesSave->FileName = gcnew String(logFileName);
		if (this->FilesSave->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			_file = this->FilesSave->FileName;

		this->Log->Clear();

		ObjectDec objdec;
		objdec.scandir(this->Log, (char*)Lines::systemToStdString(_file).c_str(),
			(char*)Lines::systemToStdString(in_catalogue).c_str(), (char*)Lines::systemToStdString(out_catalogue).c_str());

		this->Log->AppendText("�������� ��������� ������ ���������...");
	}
	private: System::Void MainMenuDocs_Click(System::Object^  sender, System::EventArgs^  e)
	{
	MessageBox::Show("��� ������������� ��������� ��������� ������� ����������������� �����(.ini),\n" + 
										"����� � ����� � ����������(material_full.ini). \n\n"+
										"������ \"��������� GM *object\": ��������� ��������� ��������� *.object ��� �����\n"+
										"�����(������������ ������� ���������). ������ *.object ������ ��� �� *_oldest.object,\n"+
										"����� *.object �������� ������(��������) ���. ��������� ���������� ��� �� ������\n"+
										"������������(�� ������� �� ������ \"������\" ��� �� ����� ��������������.\n"+
										"������ \"��������� LOD *.object\" � \"��������� HOM *.object\" ����������� *.object ���\n"+
										" LOD - ������ � HOM - ������ ��������������. ��� �� ������������.\n"+
										"������ \"��������� ������� *.object's\" �������� ������������ ����� �������, ����������\n"+
										"*.object;'s ��� ������� ������� ���������. ��������� ���������� ��� �� ������\n" +
										"������������. ��� ���������� ������ ��������� �������� ��������� ������� \n"+
										"����������� ������� ������ �����.\n\n" +
										"������ \"���������� *.object\" ��������� ������� ���������� ����� *.object.\n"+
										"������ \"��������� *.object\" ��������� ������� ���������� ����� *.object, ��������\n"+
										"��� � ����.\n\n��������� �������������.\n", "����������", MessageBoxButtons::OK,
										MessageBoxIcon::Information);
	}
	private: System::Void MainMenuAbout_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		About^ ab = gcnew About();
		ab->ShowDialog();
	}
};
}
