from django.urls import path
from .views import DhtView


urlpatterns = [
    path("", DhtView.as_view(), name="dht-view")
]