/*
 Copyright (C) 2016 Rory Walsh
 
 Cabbage is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 Cabbage is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU General Public
 License along with Csound; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 02111-1307 USA
 */

#include "CabbageUnlockButton.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"

class UnlockForm : public Component
{

public:
	UnlockForm(String serverURL, String shopURL, String caption, int trialPeriod = 30) 
		: Component()
	{
		setSize(400, 300);
		headerHeight = 80;
		product = caption;
		//logoPath        = "";
		//backgroundPath  = "";

		resourceFile = File::getCurrentWorkingDirectory().getChildFile("cabbageAuth.txt");             // File object for local authoring file in working directory
		// or : File resourceFile ("/absolute/path/to/file.txt");                                       // ??? how to determine path during installation

		// RSA public key matching private key on server parsed from Cabbage Widget/cSound
		String  publicKeyInput = "11,6054497ecab2fc864ff3176a4790b4fac395ad7cce2558a261edefa73cac1e7ba5f61baee1f2fac4fd89d36ed30f56724eb95615fd22eabfa9fdc1a4e4be401462603b38df60701c6872ca660d15483f2e2e071a04f811049a2ccdfea8d6794e33299e5751b361288956e38dd08b1afec8b70eb0fff1878370c7efcf3c523fc5";
		public_key = RSAKey(publicKeyInput);            // String to RSAKey
		buyURL = shopURL;                           // parsed from Cabbage/cSound
		licenseServer = serverURL;// "http://localhost/licenseServer.php";

		setDemoCustomer(3);

		headerHeight = headerHeight < 25 ? 25 : headerHeight;
		headerHeight = headerHeight > 80 ? 80 : headerHeight;
		resized();
		drawBasicAppearance();

		StringArray machIDarray = juce::OnlineUnlockStatus::MachineIDUtilities::getLocalMachineIDs();   // get machineIDs
		machineID = machIDarray[0];                                                              // pick one
		std::cout << "generated MACHINE ID: " << machineID << "\n";

		machineID = "1ABC2TEST";                                         // this machineID is registered with the demo accounts on the server and is only for testing

		localAuth = checkAuthorisationFile(resourceFile);                                               //call function to read file. returns "" on failure
		int licensingScore = -5;
		if (localAuth != "") {                                                                              // if a file was found
			std::cout << "local File existing...\n";
			licensingScore = validateLicense(true, localAuth);                                          // validate license from encrypted string
		}
		switch (licensingScore) {
		case 4:                                     // valid license on disk
			// UNLOCK PLUGIN
			// CLOSE THIS APP
			drawQuit();
			break;                                  // valid trial on disk
		case 2:
			drawInput(2, daysLeftInTrial);
			// wait for user input
			break;
		case 0:                                     //trial period expired on disk
			drawInput(0, daysLeftInTrial);
			//  wait for user input
			break;
		default:                                    // no file on disk -> user input -> call server
			drawInput(1, trialPeriod);
			std::cout << "waiting for user input...\n";
			break;
		}
		messageOutput.setText(message, juce::dontSendNotification);             // print message in any case
		std::cout << "output on message output label:\n" << message << "\n";
		std::cout << "licensingScore:" << licensingScore << "\n";
	};

	~UnlockForm() override
	{ 
		setLookAndFeel(nullptr); 
	}


	void resized()
	{
		titleLabel.setBounds(10, (headerHeight - 18) / 2, getWidth() - 20, 18);
		emailLabel.setBounds(10, headerHeight + 10, 67, 20);
		emailInput.setBounds(90, headerHeight + 10, getWidth() - 110, 20);
		passwordLabel.setBounds(10, headerHeight + 40, 67, 20);
		passwordInput.setBounds(90, headerHeight + 40, getWidth() - 110, 20);
		unlockButton.setBounds(90, headerHeight + 70, 45, 20);
		trialButton.setBounds(145, headerHeight + 70, 45, 20);
		trialDays.setBounds(190, headerHeight + 70, 60, 20);
		continueTrialButton.setBounds(145, headerHeight + 70, 60, 20);
		continueTrialLabel.setBounds(205, headerHeight + 70, 100, 20);
		buyButton.setBounds(getWidth() - 65, headerHeight + 70, 45, 20);
		messageOutput.setBounds(90, headerHeight + 115, getWidth() - 110, getHeight() - headerHeight - 125);
		messageLabel.setBounds(10, headerHeight + 115, 65, 20);
		okButton.setBounds(getWidth() / 2 - 20, headerHeight + 75, 40, 20);
	};


	void drawBasicAppearance() 
	{                                              // draws the basic elements of the GUI:
		addAndMakeVisible(titleLabel);                                                     // Title
		titleLabel.setFont(juce::Font(16.0f, juce::Font::bold));
		titleLabel.setText("Unlock " + product, juce::dontSendNotification);
		//titleLabel.setText ("Unlock ", juce::dontSendNotification);
		titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
		titleLabel.setJustificationType(juce::Justification::centred);

		addAndMakeVisible(messageOutput);                                                  // Output for messages
		//messageOutput.setText("multiline\ntext", juce::dontSendNotification);
		messageOutput.setColour(Label::outlineColourId, juce::Colours::white.withAlpha(0.4f));
		messageOutput.setColour(juce::Label::backgroundColourId, juce::Colours::black.withAlpha(0.5f));
		messageOutput.setJustificationType(juce::Justification::centredTop);
		messageOutput.setFont(Font(12));

		addAndMakeVisible(messageLabel);                                                   // and it's Label
		messageLabel.setText("message:", juce::dontSendNotification);
		messageLabel.setColour(juce::Label::textColourId, juce::Colours::white);
		messageLabel.setJustificationType(juce::Justification::right);

		//    File SVGFile ("/absolute/path/to/file.txt");
		//    Drawable::createFromSVGFile (SVGFile);
		//    Drawable::createFromSVGFile(File("WhiteNotehead.svg"));
		//    static Drawable *halfNote = Drawable::createFromSVGFile(File("WhiteNotehead.svg"));
	}
	

	void gotoShop()
	{
		auto shop = URL(buyURL);
		bool success = shop.URL::launchInDefaultBrowser();
		if (!success)
		{
			message = "unable to open browser";
			messageOutput.setText(message, juce::dontSendNotification);                 // print message to output
		}
	}
	void drawInput(int trialValid, int trialPeriod) 
	{                         // draws user input acustomed to neccessity
		/*
		 trial Valid cases:           options:
			0 - trial expired         (buy)
			1 - new trial available   (start trial or buy)
			2 - existing trial period (continue or buy)
		 */
		addAndMakeVisible(emailLabel);
		emailLabel.setText("user email:", juce::dontSendNotification);
		emailLabel.setColour(juce::Label::textColourId, juce::Colours::white);
		emailLabel.setJustificationType(juce::Justification::right);

		addAndMakeVisible(emailInput);
		emailInput.setText(email, dontSendNotification);
		emailInput.setEditable(true);
		emailInput.setColour(juce::Label::backgroundColourId, juce::Colours::black.withAlpha(0.5f));
		emailInput.setColour(Label::outlineColourId, Colours::white.withAlpha(0.7f));
		emailInput.setColour(Label::outlineWhenEditingColourId, Colours::white);
		emailInput.setColour(Label::textColourId, Colours::lightgrey);
		emailInput.setColour(Label::textWhenEditingColourId, Colours::white);
		emailInput.onTextChange = [this] { email = emailInput.getText(); };

		addAndMakeVisible(passwordLabel);
		passwordLabel.setText("password:", juce::dontSendNotification);
		passwordLabel.setColour(juce::Label::textColourId, juce::Colours::white);
		passwordLabel.setJustificationType(juce::Justification::right);

		addAndMakeVisible(passwordInput);
		passwordInput.setText(coverPassword(password), dontSendNotification);
		passwordInput.setEditable(true);
		passwordInput.setColour(juce::Label::backgroundColourId, juce::Colours::black.withAlpha(0.5f));
		passwordInput.setColour(Label::outlineColourId, Colours::grey);
		passwordInput.setColour(Label::outlineWhenEditingColourId, Colours::white.withAlpha(0.7f));
		passwordInput.setColour(Label::textColourId, Colours::lightgrey);
		passwordInput.setColour(Label::textWhenEditingColourId, Colours::white);
		passwordInput.onTextChange = [this] { password = passwordInput.getText(); };

		addAndMakeVisible(unlockButton);
		unlockButton.setColour(TextButton::ColourIds::buttonColourId, Colours::black.withAlpha(0.4f));
		unlockButton.setColour(TextButton::ColourIds::buttonOnColourId, Colours::white.withAlpha(0.4f));
		unlockButton.setColour(TextButton::ColourIds::textColourOffId, Colours::white);
		unlockButton.setColour(TextButton::ColourIds::textColourOnId, Colours::white);
		unlockButton.setButtonText("unlock");
		unlockButton.onClick = [this] { callServer("authorisation"); };

		trialButton.setColour(TextButton::ColourIds::buttonColourId, Colours::black.withAlpha(0.4f));
		trialButton.setColour(TextButton::ColourIds::buttonOnColourId, Colours::white.withAlpha(0.4f));
		trialButton.setColour(TextButton::ColourIds::textColourOffId, Colours::white);
		trialButton.setColour(TextButton::ColourIds::textColourOnId, Colours::white);
		trialButton.setButtonText("try");
		trialButton.onClick = [this] { callServer("trial"); };

		continueTrialButton.setColour(TextButton::ColourIds::buttonColourId, Colours::black.withAlpha(0.4f));
		continueTrialButton.setColour(TextButton::ColourIds::buttonOnColourId, Colours::white.withAlpha(0.4f));
		continueTrialButton.setColour(TextButton::ColourIds::textColourOffId, Colours::white);
		continueTrialButton.setColour(TextButton::ColourIds::textColourOnId, Colours::white);
		continueTrialButton.setButtonText("continue");
		continueTrialButton.onClick = [this] { drawQuit(); };

		buyButton.setColour(TextButton::ColourIds::buttonColourId, Colours::black.withAlpha(0.4f));
		buyButton.setColour(TextButton::ColourIds::buttonOnColourId, Colours::white.withAlpha(0.4f));
		buyButton.setColour(TextButton::ColourIds::textColourOffId, Colours::white);
		buyButton.setColour(TextButton::ColourIds::textColourOnId, Colours::white);
		buyButton.setButtonText("buy");
		buyButton.onClick = [this] { gotoShop(); };

		switch (trialValid) {
		case 0:                                                             // if an expited trial period was found either on disk or on the license server
			addAndMakeVisible(buyButton);
			trialDays.setVisible(false);
			trialButton.setVisible(false);
			break;
		case 1:                                                             // if no authorisation file is found on disk e.g. after a new installation
			if (trialPeriod > 0 && continueTrialLabel.isVisible() == false) {   // if a trial Period is wanted by the developer
				addAndMakeVisible(trialButton);                            // draw button to start a new trial period or to unlock with the users credentials
				addAndMakeVisible(trialDays);
				trialDays.setText("for " + String(trialPeriod) + " days", juce::dontSendNotification);
				trialDays.setColour(juce::Label::textColourId, juce::Colours::white);
				trialDays.setJustificationType(juce::Justification::left);
			}
			addAndMakeVisible(buyButton);                                  // otherwise draw only buy buton
			break;
		case 2:                                                             // if a running trial period is found either on disk or on the license server
			trialButton.setVisible(false);
			trialDays.setVisible(false);

			addAndMakeVisible(continueTrialButton);

			addAndMakeVisible(continueTrialLabel);
			continueTrialLabel.setText("trial: " + String(trialPeriod) + " days left.", juce::dontSendNotification);
			continueTrialLabel.setColour(juce::Label::textColourId, juce::Colours::white);
			continueTrialLabel.setJustificationType(juce::Justification::left);

			addAndMakeVisible(buyButton);
			break;
		default:
			break;
		}
	}

	void drawSuccess()                               // to be drawn in the case of a new license being activated
	{
		buyButton.setVisible(false);
		trialDays.setVisible(false);
		trialButton.setVisible(false);
		continueTrialLabel.setVisible(false);
		continueTrialButton.setVisible(false);
		unlockButton.setVisible(false);
		addAndMakeVisible(okButton);
		okButton.setButtonText("OK!");
		okButton.onClick = [this] { drawQuit(); };
	}

	void drawQuit()                                  // this is actually not to be implemented to the widget
	{                                                               // instead this is the place to unlock the plugin
		buyButton.setVisible(false);                                // and close the widget window
		trialDays.setVisible(false);
		trialButton.setVisible(false);
		continueTrialLabel.setVisible(false);
		continueTrialButton.setVisible(false);
		unlockButton.setVisible(false);
		emailLabel.setVisible(false);
		emailInput.setVisible(false);
		passwordLabel.setVisible(false);
		passwordInput.setVisible(false);
		okButton.setVisible(false);
		titleLabel.setText("Quit Unlock Widget...", juce::dontSendNotification);

	}

	String checkAuthorisationFile(File resourceFile) 
	{                                   // This function reads the textfile from the given path
		String textfile;
		if (!resourceFile.existsAsFile())
		{
			std::cout << "File doesn't exist ...";
			//textfile = "";
		}
		else {
			std::unique_ptr<FileInputStream> input(resourceFile.createInputStream());
			if (!input->openedOk())
			{
				std::cout << "Failed to open file";
			}
			else {
				while (!input->isExhausted())
				{
					String singleLine = input->readNextLine().toStdString();
					textfile = textfile + singleLine;
				}
				std::cout << "local file loaded: \n" << textfile << "\n";
			}
		}
		return textfile;
	}

	String callLicenseServer(String licenseServer, String postfields)
	{                                               // function to http request the given server
		std::cout << "Calling license server...\n";                                                                  // and return it's encrypted response
		String serverReturn;
		auto url = juce::URL(licenseServer)                                                                        // create URL objcet
			.withPOSTData(postfields);
		if (auto inputStream = url.createInputStream(URL::InputStreamOptions(URL::ParameterHandling::inPostData)  // open Stream
			.withConnectionTimeoutMs(10000)
			.withNumRedirectsToFollow(0)))
		{
			serverReturn = inputStream->readString();                                                              // parse return form server to string
			//std::cout <<"\n\nCYPHER:\n" << serverReturn <<"\n\n";
		}
		return serverReturn;
	}

	String decryptResponse(String serverReturn, RSAKey public_key) 
	{                  // function to decrypt the server response
		BigInteger bi;                                                              // create BigInt object
		bi.parseString(serverReturn, 16);                                           // parse the encrypted server message to it
		public_key.applyToValue(bi);                                                // decrypt it with public key
		String decrypted = bi.toMemoryBlock().toString();                           // convert decryption to string
		return decrypted;
	}

	int trialDaysLeft(String returnTrialStart, int trialPeriod) 
	{                                   // function to validate a trial period with the given period
		int trialYear = returnTrialStart.substring(0, 4).getIntValue();                            // and a date-string formated "YYYY-MM-DD" (chars in between don't matter)
		int trialMonth = returnTrialStart.substring(5, 7).getIntValue() - 1;
		int trialDay = returnTrialStart.substring(8, 10).getIntValue();
		Time trialEnd = juce::Time(trialYear, trialMonth, trialDay + trialPeriod + 1, 0, 0, 0, 0, true);
		Time currentDate = juce::Time::getCurrentTime();
		const juce::RelativeTime trialTimeLeft = trialEnd - currentDate;
		return trialTimeLeft.inDays();
	}

	bool writeAuthorisationFile(File resourceFile, String serverReturn)
	{            // function to write the authorising file to disk
		bool success;                                                               // after a permanent or trial license was found on the server
		TemporaryFile tempFile(resourceFile);
		FileOutputStream output(tempFile.getFile());
		if (!output.openedOk())
		{
			DBG("FileOutputStream didn't open correctly ...");
		}
		output.setNewLineString("\n");
		output.writeText(serverReturn, 1, 1, nullptr);
		output.flush(); // (called explicitly to force an fsync on posix)
		if (output.getStatus().failed())
		{
			DBG("An error occurred in the FileOutputStream");
		}
		success = tempFile.overwriteTargetFileWithTemporary();
		return success;
	}


	int validateLicense(bool localFileAuth, String serverReturn)
	{                    // base method for validating a license
		bool success = false;
		int valid;
		if (localFileAuth) {
			std::cout << "Validating from local file\n";
		}
		else {
			std::cout << "Validating from server\n";
		}
		std::cout << "with public key :\n" << public_key.toString() << "\n";

		String decrypted = decryptResponse(serverReturn, public_key);                                //decrypt server response with public_key
		std::cout << "DECRYPTED:\n" << decrypted << "\n";

		var jsonReply = JSON::parse(decrypted);                                                     // create JSON object and parse decrypted server response
		String returnMessage = jsonReply.getProperty("message", var()).toString();               // assign JSON proprety to vars
		String returnEmail = jsonReply.getProperty("email", var()).toString();
		String returnProduct = jsonReply.getProperty("product", var()).toString();
		String returnMachineID = jsonReply.getProperty("machineID", var()).toString();
		String returnLicense = jsonReply.getProperty("license", var()).toString();
		String returnTrialStart;                                                                    // don't search for "trial start" JSON property yet
		String returnTrialPeriod;                                                                   // don't search for "trial period" JSON property yet

		if (returnLicense == "trial") {                                                               // query if license request is "trial"
			returnTrialStart = jsonReply.getProperty("trialStart", var()).toString();            // assign JSON proprety for trial starting date
			returnTrialPeriod = jsonReply.getProperty("trialPeriod", var()).toString();           // assign JSON proprety for trial starting date
			int trialPeriodFromServer = returnTrialPeriod.getIntValue();                            // and get trial period length from server for safety
			daysLeftInTrial = trialDaysLeft(returnTrialStart, trialPeriodFromServer);              // get days left in trial period
			if (daysLeftInTrial > 0) {                                                                // check if trial period is valid
				if (localFileAuth == false) {                                                           // check if newly unlocking from SERVER (local auth. file = false)
					if (bool success = writeAuthorisationFile(resourceFile, serverReturn)) {          //Successfully wrote local authoring file to disk
						std::cout << "\nTrial started on   : " << returnTrialStart << "\n";           //bla bla...
						std::cout << "Days left in trial : " << daysLeftInTrial << "\n\n";
						message = returnMessage;
						//UNLOCK PLUGIN...
						success = true;
						valid = 1;                                                                  // return 1 for validation as new trial sent from server
					}
					else {                                                                        // server validated trial failed to be written to auth. file to disk
						std::cout << "\nError writing authoting file to disk!\n\n";                  //bla bla...
						message = "Error writing authoring file for trail to disk!";
						//BACK TO CREDENTIAL INPUT...
						success = false;
						valid = -3;                                                                 // return -3 = validation fail because trial couldn't be writen to disk
					}
				}
				else {                                                                            //unlocking from local auth. file as trial
					std::cout << "\nTrial started on   : " << returnTrialStart << "\n";               //bla bla...
					std::cout << "Days left in trial : " << daysLeftInTrial << "\n\n";
					message = "Trial started on   : " + returnTrialStart + "\nDays left in trial : " + String(daysLeftInTrial);
					//UNLOCK PLUGIN...
					success = true;
					valid = 2;                                                                      // return 2 for validation as running trial on didk
				}

			}
			else {                                                                                // 0 or less days left in trial period
				std::cout << "\nTrial period expired.\n\n";
				message = "Trial period expired!";
				//BACK TO CREDENTIAL INPUT...
				success = false;
				valid = 0;                                                                          // return 0 for trial period expired through local auth. file or server
			}
		}
		else if (returnLicense == "permanent") {
			if (email == returnEmail && product == returnProduct && machineID == returnMachineID) {   //check if response matches credentials
				if (localFileAuth == false) {                                                           //check if newly unlocking from SERVER (local auth. file = false)
					if (bool success = writeAuthorisationFile(resourceFile, serverReturn)) {          //Successfully wrote local authoring file to disk
						std::cout << returnProduct << " unlocked for " << returnEmail << "\n";
						message = returnMessage;
						//UNLOCK PLUGIN...
						success = true;
						valid = 3;                                                                  // return 3 for new valid license from server
					}
					else {                                                                        // server validated license failed to be written to auth. file to disk
						std::cout << "\nError writing authoting file to disk!\n\n";
						message = "Error writing authoting file to disk!";
						//BACK TO CREDENTIAL INPUT...
						success = false;
						valid = -4;                                                                 // return -4 = validation fail because license couldn't be writen to disk
					}
				}
				else {                                                                                //unlocking from local auth. file
					std::cout << "\nopening " << returnProduct << " licensed to " << returnEmail << "\n"; //bla bla...
					message = "opening " + returnProduct + " licensed to " + returnEmail;
					//UNLOCK PLUGIN...
					success = true;
					valid = 4;                                                                          // return 4 for valid license on disk
				}

			}
			else {                                                                                  // credentials on server didn't match
				std::cout << "\nAuthorisation failed!\n";
				std::cout << "Message from Server:\n" << returnMessage << "\n\n";
				message = returnMessage;
				//BACK TO CREDENTIAL INPUT...
				success = false;
				valid = -1;                                                                         // return -1 for validation failed due to credetials not matching
			}
		}
		else {                                                                                      // no license type was returned
			std::cout << "\nAuthorisation failed!\n";
			std::cout << "\nServer reporting error:\n" << returnMessage << "\n\n";
			message = returnMessage;
			//BACK TO CREDENTIAL INPUT...
			success = false;
			valid = -2;                                                                             // return -2 for user login fail or purchase not in account
		}
		return valid;
		/*
		 return 4   : success - valid license on disk
		 return 3   : success - new valid license from server written to disk
		 return 2   : success - valid trial on disk
		 return 1   : success - valid new trial from server written to disk
		 return 0   : fail    - trial period expired on disk and server
		 return -1  : fail    - credetials and/or machineID not matching
		 return -2  : fail    - user login failed or purchase not in account
		 return -3  : fail    - valid trial from server could not be writen to disk
		 return -4  : fail    - valid license from server could not be writen to disk
		 */
	}

	int callServer(String request)                                   // method to be called by the buttons "unlock" or "start trial"
	{                                                                               // to start off the online licensing process
		int licensingScore = -5;
		String postfields = "email=" + email +                              // concat $POST fields string for HTTP request
			"&password=" + password +
			"&product=" + product +
			"&machineID=" + machineID +
			"&request=" + request;
		String serverReturn = callLicenseServer(licenseServer, postfields);          //call function to http-request server
		if (serverReturn != "") {
			std::cout << "\nEncrypted Server Response:\n" << serverReturn << "\n";
			licensingScore = validateLicense(false, serverReturn);
			switch (licensingScore) {
			case 3:                                                             // new valid license from server written to disk
				std::cout << "\n*** SUCCESS ***\n";
				drawSuccess();
				//
				break;
			case 1:                                                             //valid new trial from server written to disk
				drawInput(2, daysLeftInTrial);
				//
				break;
			case 0:                                                             //trial period expired on server
				drawInput(0, daysLeftInTrial);
				//
				break;
			case -1:                                                            //credetials and/or machineID not matching
				drawInput(1, daysLeftInTrial);
				//
				break;
			case -2:                                                            //user login failed or purchase not in account
				drawInput(1, daysLeftInTrial);
				//
				break;
			case -3:                                                            //valid trial from server could not be writen to disk
				drawInput(1, daysLeftInTrial);
				//
				break;
			case -4:                                                            //valid license from server could not be writen to disk
				drawInput(1, daysLeftInTrial);
				//
				break;
			}
		}
		else {
			std::cout << "\nUnable to reach license server";                         // server was not reachable
			std::cout << "\nCheck Internet connection / firewall settings\n\n";
			//BACK TO CREDENTIAL INPUT...
		}
		messageOutput.setText(message, juce::dontSendNotification);                 // print message to output
		std::cout << "output on message output label:\n" << message << "\n";
		std::cout << "\nlicensingScore:" << licensingScore << "\n";
		return licensingScore;
	}

	void setDemoCustomer(int customerID) 
	{
		switch (customerID) {
		case 1:
			email = "theeinstein@emcq.com";       // 1 valid license
			password = "theeinstein";                // no trial period registered
			product = "Broccoli Plugin";
			break;
		case 2:
			email = "thetesla@energy.xyz";        // no valid license (payment didn't come through)
			password = "thetesla";                   // no trial period registered
			product = "Broccoli Plugin";
			break;
		case 3:
			email = "davinci@renaissance.xyz";    // 2 valid licenses in 1 purchase
			password = "davinci";                    // expired trial period in account (this one is not reset by the resetCustomers.php)
			product = "Broccoli Plugin";
			break;
		case 4:
			email = "alex@empire.big";            // 4 valid licenses in 2 purcheses
			password = "alex";                       // no trial period registered
			product = "Broccoli Plugin";
			break;
		case 5:
			email = "mail@deutschersoul.de";      // no license and
			password = "captainhook";                // expired trial period
			product = "Cabbage Plugin";
			break;
		default:
			break;
		}
	}

	String coverPassword(String password)
	{                                                   // function to create the typical "cover" for passwords
		String coveredPW;
		int PWlen = password.length();
		for (int cnt = 0; cnt < PWlen; cnt++) {
			coveredPW += "*";                                                               // if the dots are not working in Cabbage use "*".
		}
		return coveredPW;
	}

	/*void paint(Graphics& g) {
			g.fillAll(Colours::red);
	}*/

private:
	Label titleLabel;
	Label emailLabel;
	Label emailInput;
	Label passwordLabel;
	Label passwordInput;
	Label messageOutput;
	Label messageLabel;
	TextButton unlockButton;
	TextButton trialButton;
	TextButton buyButton;
	Label trialDays;
	TextButton continueTrialButton;
	Label continueTrialLabel;
	TextButton okButton;
	String caption;
	String email;
	String password;
	String product;
	String message;
	String buyURL;
	int trialPeriod;

	RSAKey public_key;
	String licenseServer;
	String serverReturn;
	String localAuth;
	File resourceFile;
	int daysLeftInTrial;
	String machineID;
	int headerHeight = 100;
};

CabbageUnlockButton::CabbageUnlockButton(ValueTree wData, CabbagePluginEditor* _owner)
	: TextButton(),
	owner(_owner),
	widgetData(wData),
	CabbageWidgetBase(_owner)
{
	widgetData.addListener(this);              //add listener to valueTree so it gets notified when a widget's property changes
	initialiseCommonAttributes(this, wData);   //initialise common attributes such as bounds, name, rotation, etc..
	setButtonText(getTextArray()[getValue()]);
	addListener(this);
	tooltipText = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::popuptext);
	if (tooltipText.isNotEmpty())
		setTooltip(tooltipText);

	setClickingTogglesState(true);

	setToggleState((bool)getValue(), dontSendNotification);

	auto csdPath = owner->getProcessor().getCsdFile().getFullPathName();
	setImgProperties(*this, wData, csdPath, "buttonon");
	setImgProperties(*this, wData, csdPath, "buttonoff");
	setImgProperties(*this, wData, csdPath,  "buttonover");

	const String imgOff = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::imgbuttonoff);
	const String imgOver = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::imgbuttonover);
	const String imgOn = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::imgbuttonon);

	setLookAndFeelColours(wData);

	getProperties().set("outlinecolour", CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::outlinecolour));
	getProperties().set("outlinethickness", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::outlinethickness));
	getProperties().set("corners", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::corners));

	if (owner->globalStyle == "legacy")
	{
		return;
	}



	//if users are passing custom images, use old style look and feel
	if (CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::style) == "flat" &&
		imgOff.isEmpty() && imgOn.isEmpty() && imgOver.isEmpty())
	{
		int fontstyle = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::fontstyle);
		owner->customFont.setStyleFlags(fontstyle);
		flatLookAndFeel.customFont = owner->customFont;
		setLookAndFeel(&flatLookAndFeel);
		lookAndFeelChanged();
	}
	else {
		lookAndFeel.customFont = owner->customFont;
		setLookAndFeel(&lookAndFeel);
		lookAndFeelChanged();
	}


}

void CabbageUnlockButton::buttonClicked(Button*)
{
	const String serverURL = CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::serverURL);
	const String shopURL = CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::shopURL);
	const String caption = CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::caption);
	DialogWindow::LaunchOptions o;
	auto content = std::make_unique<UnlockForm>(serverURL, shopURL, caption);
	content->setSize(400, 300);
	o.content.setOwned(content.release());
	o.dialogTitle = TRANS("Unlock / Register");
	o.dialogBackgroundColour = o.content->getLookAndFeel().findColour(ResizableWindow::backgroundColourId);
	o.escapeKeyTriggersCloseButton = true;
	o.useNativeTitleBar = true;
	o.resizable = false;
	o.launchAsync();
}

void CabbageUnlockButton::setLookAndFeelColours(ValueTree wData)
{
	setColour(TextButton::textColourOffId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::fontcolour)));
	setColour(TextButton::buttonColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::colour)));
	setColour(TextButton::textColourOnId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::onfontcolour)));
	setColour(TextButton::buttonOnColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::oncolour)));
	getProperties().set("outlinecolour", CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::outlinecolour));
	getProperties().set("outlinethickness", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::outlinethickness));
	getProperties().set("corners", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::corners));
	repaint();
}

void CabbageUnlockButton::valueTreePropertyChanged(ValueTree& valueTree, const Identifier& prop)
{
	setLookAndFeelColours(valueTree);
	handleCommonUpdates(this, valueTree, false, prop);      //handle comon updates such as bounds, alpha, rotation, visible, etc
	populateTextArrays(valueTree);
	setButtonText(getTextArray()[getValue()]);
	setTooltip(getCurrentPopupText(valueTree));
}



//===============================================================================


