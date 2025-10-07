#pragma once

#include <QDialog>
#include "ui_loadindexdialog.h"

#include <filesystem>
namespace fs = std::filesystem;

#include "gamedatainfo.hpp"

class CMainWindow;

class CLoadIndexDialog : public QDialog, public Ui::LoadIndexDialog
{
    Q_OBJECT

public:
    CLoadIndexDialog( CMainWindow* pParent, const QString& lastIndexDir = {} );

private slots:
    void OnBrowseGameDir();
    void OnGameDirLineEdited( const QString& text );

    void OnOkayButton();

private:
    void UpdateGameProviderLabel( fs::path newDirPath );

private:
    GameDataInfo m_DetectedGameInfo;
    QString m_LastIndexDir;
};
