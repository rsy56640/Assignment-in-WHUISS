Public Class OptionForm
    Private myPaint As New Form1
    Private Sub ComboBox1_SelectedIndexChanged(sender As Object, e As EventArgs) Handles ComboBox1.SelectedIndexChanged
        myPaint.setSize(ComboBox1.SelectedIndex)
    End Sub

    Private Sub OptionForm_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        myPaint.Show()
        Label2.Text = myPaint.PaintCanvas1.GraphicsColor.ToString()
    End Sub

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        ColorDialogBrush.ShowDialog()
        Try
            myPaint.setColor(ColorDialogBrush.Color)
            Label2.Text = myPaint.PaintCanvas1.GraphicsColor.ToString()
        Catch ex As Exception
            MessageBox.Show(ex.Message)
        End Try
    End Sub

    Private Sub btnClear_Click(sender As Object, e As EventArgs) Handles btnClear.Click
        myPaint.PaintCanvas1.clearCanvas()
    End Sub
End Class