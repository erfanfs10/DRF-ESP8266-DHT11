from django.contrib import admin
from .models import Dht


@admin.register(Dht)
class DhtAdmin(admin.ModelAdmin):
    list_display = ("temp", "humi", "created")
    list_filter = ("created", )