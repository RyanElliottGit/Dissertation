#pragma once
/*
#include "pch.h"
#include "Card.h"
#include "Deck.h"
#include <ctime>
#include <iostream>
#include <string>
#include <boost\asio.hpp>
#include <boost\array.hpp>
#include <boost\enable_shared_from_this.hpp>
#include <boost\bind.hpp>
#include <chrono>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <dos.h>
#include <Windows.h>
#include <map>

using boost::asio::ip::tcp;
using namespace std;
using namespace SuitNamespace;
using namespace FaceNamespace;

enum { max_length = 1024 };

*/
namespace EchoGuiClient {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  draw;
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->draw = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(47, 39);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(57, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Draw Card";
			this->label1->Click += gcnew System::EventHandler(this, &MyForm::label1_Click);
			// 
			// draw
			// 
			this->draw->Location = System::Drawing::Point(50, 77);
			this->draw->Name = L"draw";
			this->draw->Size = System::Drawing::Size(54, 23);
			this->draw->TabIndex = 1;
			this->draw->Text = L"Draw";
			this->draw->UseVisualStyleBackColor = true;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(628, 266);
			this->Controls->Add(this->draw);
			this->Controls->Add(this->label1);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
	}
	};
}

/*
int main()
{
	Deck deck = Deck();
	try
	{
		string port = "300";
		//string ip = "192.168.1.172";
		cout << "Enter the ip of the host:";
		string ip;
		cin >> ip;

		cin.ignore();

		boost::asio::io_service io_service;

		tcp::resolver resolver(io_service);
		tcp::resolver::query query(tcp::v4(), ip, port);
		tcp::resolver::iterator iterator = resolver.resolve(query);

		tcp::socket s(io_service);
		boost::asio::connect(s, iterator);

		int i = 0;
		while (i < 5)
		{
			cout << "Enter message: ";
			char request[max_length];
			cin.getline(request, max_length);
			size_t request_length = strlen(request);
			boost::asio::write(s, boost::asio::buffer(request, request_length));

			char reply[max_length];
			size_t reply_length = boost::asio::read(s,
				boost::asio::buffer(reply, request_length));
			cout << "Reply is: ";
			cout.write(reply, reply_length);
			cout << "\n";
			i++;
		}
	}
	catch (exception& e)
	{
		cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}
*/