Public Class GraphicCircle
    Inherits GraphicsItem

    Public Overrides Sub Draw(graphics As Graphics)
        Dim objSolidBrush = New SolidBrush(Me.color)
        graphics.FillEllipse(objSolidBrush, Me.rectangle)
    End Sub
End Class
