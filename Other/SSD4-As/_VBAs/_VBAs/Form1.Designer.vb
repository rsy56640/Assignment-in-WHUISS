<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Form1
    Inherits System.Windows.Forms.Form

    'Form 重写 Dispose，以清理组件列表。
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Windows 窗体设计器所必需的
    Private components As System.ComponentModel.IContainer

    '注意: 以下过程是 Windows 窗体设计器所必需的
    '可以使用 Windows 窗体设计器修改它。  
    '不要使用代码编辑器修改它。
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.PaintCanvas1 = New _VBAs.PaintCanvas()
        Me.SuspendLayout()
        '
        'PaintCanvas1
        '
        Me.PaintCanvas1.BackColor = System.Drawing.Color.White
        Me.PaintCanvas1.Dock = System.Windows.Forms.DockStyle.Fill
        Me.PaintCanvas1.Location = New System.Drawing.Point(0, 0)
        Me.PaintCanvas1.Name = "PaintCanvas1"
        Me.PaintCanvas1.Size = New System.Drawing.Size(691, 464)
        Me.PaintCanvas1.TabIndex = 0
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(8.0!, 15.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(691, 464)
        Me.Controls.Add(Me.PaintCanvas1)
        Me.Name = "Form1"
        Me.Text = "Form1"
        Me.ResumeLayout(False)

    End Sub

    Friend WithEvents PaintCanvas1 As PaintCanvas
End Class
