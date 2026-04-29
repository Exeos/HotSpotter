package me.exeos.hotspotter.event;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class EventManager {

    private final HashMap<Object, HashMap<Class<? extends Event>, List<Method>>> subs = new HashMap<>();

    public void subscribe(Object target) {
        HashMap<Class<? extends Event>, List<Method>> tm = new HashMap<>();

        for (Method method : target.getClass().getDeclaredMethods()) {
            if (method.isAnnotationPresent(EventHandler.class) && method.getParameterCount() == 1 && Event.class.isAssignableFrom(method.getParameterTypes()[0])) {
                method.setAccessible(true);

                @SuppressWarnings("unchecked")
                Class<? extends Event> clazz = (Class<? extends Event>) method.getParameterTypes()[0];
                tm.computeIfAbsent(clazz, k -> new ArrayList<>()).add(method);
            }
        }

        subs.put(target, tm);
    }

    public void unsubscribe(Object target) {
        subs.remove(target);
    }

    public void fire(Event event) {
        for (Object object : subs.keySet()) {
            var map = subs.get(object);
            if (map == null) {
                continue;
            }

            var methods = map.get(event.getClass());
            if (methods == null) {
                continue;
            }

            for (Method method : methods) {
                try {
                    method.invoke(object, event);
                } catch (IllegalAccessException e) {
                    throw new RuntimeException(e);
                } catch (InvocationTargetException e) {
                    throw new RuntimeException(e);
                }
            }
        }
    }
}
