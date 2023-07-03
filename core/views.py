from rest_framework.views import APIView
from rest_framework.response import Response
from rest_framework import status
from .serializers import DhtSerializer
from .models import Dht


class DhtView(APIView):

    def post(self, request):

        if request.data["api_key"] != "@3fkOby&ydn3)pdj+(2jn)<d6gLZerEk;*58ev":
            return Response(status=status.HTTP_400_BAD_REQUEST)

        serializer = DhtSerializer(data=request.data)
        if serializer.is_valid():
            serializer.save()
            return Response(status=status.HTTP_201_CREATED)
        return Response(status=status.HTTP_400_BAD_REQUEST)


    def get(self, request):

        data = Dht.objects.first()
        serializer = DhtSerializer(data)
        return Response(serializer.data)
