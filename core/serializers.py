from rest_framework import serializers
from .models import Dht


class DhtSerializer(serializers.ModelSerializer):
    class Meta:
        model = Dht
        fields = ("temp", "humi")
