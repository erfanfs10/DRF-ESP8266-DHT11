from django.db import models


class Dht(models.Model):

    temp = models.CharField(max_length=10)
    humi = models.CharField(max_length=10)
    created = models.DateTimeField(auto_now=True)

    class Meta:
        ordering = ('-created',)

    def __str__(self) -> str:
        return f"temp : {self.temp} -- humi : {self.humi}"