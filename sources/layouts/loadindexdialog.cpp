#include "loadindexdialog.hpp"

#include <QFileDialog>
#include <QProcessEnvironment>

#include "mainwindow.hpp"

CLoadIndexDialog::CLoadIndexDialog( CMainWindow* pParent /*= nullptr*/ )
    : QDialog( pParent ), m_DetectedGameInfo()
{
    this->setupUi( this );

    this->connect( this->pbFindGameDir, SIGNAL( pressed() ), this,
                   SLOT( OnBrowseGameDir() ) );
    this->connect( this->leGameDir, SIGNAL( textEdited( const QString& ) ),
                   this, SLOT( OnGameDirLineEdited( const QString& ) ) );

    this->connect( this->buttonBox, SIGNAL( accepted() ), this,
                   SLOT( OnOkayButton() ) );
    this->connect( this->buttonBox, SIGNAL( rejected() ), this,
                   SLOT( close() ) );

    this->buttonBox->button( QDialogButtonBox::Ok )->setEnabled( false );

    if ( QProcessEnvironment::systemEnvironment()
        .contains( QStringLiteral( "FLATPAK_ID" ) ) )
    {
        // On Flatpak one cannot manually enter a path from host,
        // so by default block text entry (only in constructor, so that
        // if the user wipes the field contents later manually, they'll
        // still be able to input something manually from then on)
        this->leGameDir->setEnabled( false );
        this->OnGameDirLineEdited( this->leGameDir->text() );
    }
}

void CLoadIndexDialog::OnBrowseGameDir()
{
    QString szDirPath = QFileDialog::getExistingDirectory(
        this, tr( "Select the game's directory" ), QDir::homePath() );

    if ( szDirPath.isEmpty() == true )
    {
        return;
    }

    this->leGameDir->setText( szDirPath );

    // Allow adjusting the line again if it was previously disabled for Flatpak
    // (the user will now see the paths are sandboxed).
    this->leGameDir->setEnabled( true );

    this->UpdateGameProviderLabel( szDirPath.toStdString() );
}

void CLoadIndexDialog::OnGameDirLineEdited( const QString& text )
{
    if ( text.isEmpty() == true )
    {
        if ( QProcessEnvironment::systemEnvironment()
            .contains( QStringLiteral( "FLATPAK_ID" ) ) )
        {
            // On Flatpak one cannot manually enter a path from host,
            // so by default block text entry and use a different text.
            this->lblDetectStatus->setText( tr( "No directory selected. "
                "Select a directory by clicking \"Browse\"." ) );
        }
        else
        {
            this->lblDetectStatus->setText( tr( "No directory selected." ) );
        }

        this->buttonBox->button( QDialogButtonBox::Ok )->setEnabled( false );
        return;
    }

    this->UpdateGameProviderLabel( text.toStdString() );
}

void CLoadIndexDialog::OnOkayButton()
{
    this->close();
    static_cast<CMainWindow*>( this->parent() )
        ->OnIndexFileAccepted( this->m_DetectedGameInfo );
}

void CLoadIndexDialog::UpdateGameProviderLabel( fs::path newDirPath )
{
    fs::path newDataPath = newDirPath;

    this->m_DetectedGameInfo.SetGameDataPath( newDataPath );

    if ( this->m_DetectedGameInfo.WasGameDetected() == false )
    {
        newDataPath /= "Data";
        this->m_DetectedGameInfo.SetGameDataPath( newDataPath );

        if ( this->m_DetectedGameInfo.WasGameDetected() == false )
        {
            this->lblDetectStatus->setText(
                tr( "Could not detect any compatible game in the "
                    "directory." ) );
            this->buttonBox->button( QDialogButtonBox::Ok )
                ->setEnabled( false );
            return;
        }
    }

    QString newLabelFmt( tr( "%1's %2 detected!" ) );

    this->lblDetectStatus->setText( newLabelFmt.arg(
        this->m_DetectedGameInfo.GetPrintableProviderName().data(),
        this->m_DetectedGameInfo.GetPrintableGameName().data() ) );
    this->buttonBox->button( QDialogButtonBox::Ok )->setEnabled( true );
}
