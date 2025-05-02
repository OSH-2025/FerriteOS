import asyncio
import websockets

# handle receive text and response
async def RecvMsg(websocket):
    while True:
        recvText = await websocket.recv()
        print(recvText)
        responseText = f"your submit context: {recvText}"
        await websocket.send(responseText)

# main logic function
async def MainLogic(websocket, path):
    await RecvMsg(websocket)

# creat websockets server, please change ip as your ip address.
ip = ''
startServer = websockets.serve(MainLogic, ip, 8000)

# start event loop and set run forever
asyncio.get_event_loop().run_until_complete(startServer)
asyncio.get_event_loop().run_forever()
