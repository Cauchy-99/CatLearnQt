
var Dir = new function () {
    this.toNativeSparator = function (path) {
        if (systemInfo.productType === "windows")
            return path.replace(/\//g, '\\');
        return path;
    }
};

var CatTools = new function () {
    this.DecisionPath = function(path, widget) {
        if (systemInfo.productType === "windows")
        {
            var strs = path.split("\\");
            if(path == Dir.toNativeSparator(installer.value("ApplicationsDir"))
                || path == Dir.toNativeSparator(installer.value("ApplicationsDirX86"))
                || (strs.length == 2 && strs[1] == ""))
            {
                widget.targetDirectory.text = Dir.toNativeSparator(installer.value("ApplicationsDirX86")) + "\\" + installer.value("ProductName");
                return false;
            }
        }
        widget.complete = true;
        installer.setValue("TargetDir", path);
        return true;
    }
}



function operationForLinux()
{
    var params = "Name=CatSoftwareStarter" + "\n";
    params += "Icon=@TargetDir@/../../Resource/logo.png" + "\n";
    params += "Version=@Version@" + "\n";
    params += "Exec=@TargetDir@/CatSoftwareStarter" + "\n";
    params += "Terminal=false" + "\n";
    params += "Encoding=UTF-8" + "\n";
    params += "Type=Application" + "\n";
    params += "Categories=Network" + "\n";
 
    var desktop = "CatSoftwareStarter.desktop";
    component.addOperation("CreateDesktopEntry", desktop, params);
 
    var homeDir = installer.environmentVariable("HOME");
    if(homeDir.length > 0)
    {
        var XDG_DATA_HOME = installer.environmentVariable("XDG_DATA_HOME"); //获取XDG_DATA_HOME环境变量
        if(XDG_DATA_HOME.length > 0)
        {
	        var XDG_DESKTOP_DIR = installer.environmentVariable("XDG_DESKTOP_DIR");
            component.addOperation("Copy", XDG_DATA_HOME+"/applications/" + desktop, homeDir + XDG_DESKTOP_DIR);
        }
    }
};

function operationForWindows()
{
    //开始菜单快捷方式
    component.addOperation("CreateShortcut",                // 创建快捷方式
        "@TargetDir@/CatWidgetExamples.exe",    // 默认安装目录/CatWidgetExamples.exe
        "@StartMenuDir@/CatWidgetExamples.lnk", //开始菜单
        "workingDirectory=@TargetDir@");
        
    component.addOperation("CreateShortcut",                // 创建快捷方式
        "@TargetDir@/MaintenanceTool.exe",    // 默认安装目录/MaintenanceTool.exe
        "@StartMenuDir@/MaintenanceTool.lnk", //开始菜单
        "workingDirectory=@TargetDir@");

    //桌面快捷方式
    component.addOperation("CreateShortcut",
        "@TargetDir@/CatWidgetExamples.exe",
        "@DesktopDir@/CatWidgetExamples.lnk",
        "workingDirectory=@TargetDir@");
};

function Component()
{
    if (installer.isInstaller()) {
        component.loaded.connect(this, Component.prototype.installerLoaded);
        ComponentSelectionPage = gui.pageById(QInstaller.ComponentSelection);

        installer.setDefaultPageVisible(QInstaller.TargetDirectory, false);
        // installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
        installer.setDefaultPageVisible(QInstaller.LicenseCheck, false);
        // if (systemInfo.productType === "windows")
        //     installer.setDefaultPageVisible(QInstaller.StartMenuSelection, false);
        // installer.setDefaultPageVisible(QInstaller.ReadyForInstallation, false);
    } else if(installer.isUninstaller()) {
        QMessageBox.question("quit.question", "Uninstaller", "Uninstaller",
                                   QMessageBox.Yes | QMessageBox.No);
    }
};

Component.prototype.installerLoaded = function () {
    if (installer.addWizardPage(component, "TargetWidget", QInstaller.TargetDirectory)) {
        var widget = gui.pageWidgetByObjectName("DynamicTargetWidget");
        if (widget != null) {
            widget.targetChooser.clicked.connect(this, Component.prototype.chooseTarget);
            widget.targetDirectory.textChanged.connect(this, Component.prototype.targetChanged);

            //widget.windowTitle = "Installation Folder";
            if (systemInfo.productType === "windows")
            {
                widget.targetDirectory.text = Dir.toNativeSparator(installer.value("ApplicationsDirX86")) + "\\" + installer.value("ProductName");
            }
        }
    }

    if (installer.addWizardPage(component, "LicenseWidget", QInstaller.LicenseCheck)) {
        var widget = gui.pageWidgetByObjectName("DynamicLicenseWidget");
        if (widget != null) {
            widget.acceptLicense.toggled.connect(this, Component.prototype.checkAccepted);
            widget.complete = false;
            widget.declineLicense.checked = true;
            //widget.windowTitle = "License Agreement";
        }
    }
};

Component.prototype.targetChanged = function (text) {
    var widget = gui.pageWidgetByObjectName("DynamicTargetWidget");
    if (widget != null) {
        if (text != "") {
            CatTools.DecisionPath(text, widget);
            return;
        }
        widget.complete = false;
    }
}

Component.prototype.chooseTarget = function () {
    var widget = gui.pageWidgetByObjectName("DynamicTargetWidget");
    if (widget != null) {
        var newTarget = QFileDialog.getExistingDirectory("Choose your target directory.", widget
            .targetDirectory.text);
        if (newTarget != "")
            widget.targetDirectory.text = Dir.toNativeSparator(newTarget);
    }
}

Component.prototype.checkAccepted = function (checked) {
    var widget = gui.pageWidgetByObjectName("DynamicLicenseWidget");
    if (widget != null)
    {
        widget.complete = checked;
    }
}


Component.prototype.uninstallationFinishedPageIsShown = function()
{
    QMessageBox.question( "quit.question" ,  "isUninstaller" ,  "卸载程序" , 
    QMessageBox . Yes |  QMessageBox . No);
}

Component.prototype.createOperations = function()
{
    component.createOperations(); 
    if (systemInfo.productType === "windows") 
	{
        operationForWindows();
    } else if(systemInfo.productType === "linux")
    {
        operationForLinux();
    }
};

Component.prototype.IntroductionPageCallback = function()
{
    QMessageBox.question("quit.question", "Uninstaller", "Uninstaller",
    QMessageBox.Yes | QMessageBox.No);
    console.log("Is Updater: " + installer.isUpdater());
    console.log("Is Uninstaller: " + installer.isUninstaller());
    console.log("Is Package Manager: " + installer.isPackageManager());
}